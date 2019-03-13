// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

// commands.h : Implementation of commands of the interactive spell checking session

#pragma once

#include "util.h"

#include <spellcheck.h>
#include <objidl.h>
#include <stdio.h>

inline HRESULT ReadSingleWord(_In_ PCWSTR buffer, const size_t maxWordSize, _Out_writes_(maxWordSize) PWSTR word)
{
    int count = swscanf_s(buffer, L"%s", word, static_cast<unsigned int>(maxWordSize));
    HRESULT hr = (1 == count) ? S_OK : E_FAIL;
    if (SUCCEEDED(hr))
    {
        _Analysis_assume_nullterminated_(word);
    }

    PrintErrorIfFailed(L"ReadSingleWord", hr);
    return hr;
}

inline HRESULT ReadTwoWords(_In_ PCWSTR buffer, const size_t maxFirstSize, _Out_writes_(maxFirstSize) PWSTR first, const size_t maxSecondSize, _Out_writes_(maxSecondSize) PWSTR second)
{
    int count = swscanf_s(buffer, L"%s %s", first, static_cast<unsigned int>(maxFirstSize), second, static_cast<unsigned int>(maxSecondSize));
    HRESULT hr = (2 == count) ? S_OK : E_FAIL;
    if (SUCCEEDED(hr))
    {
        _Analysis_assume_nullterminated_(first);
        _Analysis_assume_nullterminated_(second);
    }

    PrintErrorIfFailed(L"ReadTwoWords", hr);
    return hr;
}

inline HRESULT ReadInteger(_In_ PCWSTR buffer, _Out_ int* integer)
{
    int count = swscanf_s(buffer, L"%d", integer);
    HRESULT hr = (1 == count) ? S_OK : E_FAIL;
    PrintErrorIfFailed(L"ReadInteger", hr);
    return hr;
}

inline HRESULT ReadText(_In_ PCWSTR buffer, const size_t maxTextSize, _Out_writes_(maxTextSize) PWSTR text)
{
    int count = swscanf_s(buffer, L" %[^\n]", text, static_cast<unsigned int>(maxTextSize));
    HRESULT hr = (1 == count) ? S_OK : E_FAIL;
    if (SUCCEEDED(hr))
    {
        _Analysis_assume_nullterminated_(text);
    }

    PrintErrorIfFailed(L"ReadText", hr);
    return hr;
}

inline HRESULT AddCommand(_In_ ISpellChecker* spellChecker, _In_ PCWSTR buffer)
{
    wchar_t word[MAX_PATH];
    HRESULT hr = ReadSingleWord(buffer, ARRAYSIZE(word), word);
    if (SUCCEEDED(hr))
    {
        hr = spellChecker->Add(word);
    }
    PrintErrorIfFailed(L"AddCommand", hr);
    return hr;
}

inline HRESULT IgnoreCommand(_In_ ISpellChecker* spellChecker, _In_ PCWSTR buffer)
{
    wchar_t word[MAX_PATH];
    HRESULT hr = ReadSingleWord(buffer, ARRAYSIZE(word), word);
    if (SUCCEEDED(hr))
    {
        hr = spellChecker->Ignore(word);
    }
    PrintErrorIfFailed(L"IgnoreCommand", hr);
    return hr;
}

inline HRESULT AutoCorrectCommand(_In_ ISpellChecker* spellChecker, _In_ PCWSTR buffer)
{
    wchar_t from[MAX_PATH];
    wchar_t to[MAX_PATH];
    HRESULT hr = ReadTwoWords(buffer, ARRAYSIZE(from), from, ARRAYSIZE(to), to);
    if (SUCCEEDED(hr))
    {
        hr = spellChecker->AutoCorrect(from, to);
    }
    PrintErrorIfFailed(L"AutoCorrectCommand", hr);
    return hr;
}

inline HRESULT CheckCommand(_In_ ISpellChecker* spellChecker, _In_ PCWSTR buffer)
{
    wchar_t text[MAX_PATH];
    IEnumSpellingError* enumSpellingError = nullptr;
    HRESULT hr = ReadText(buffer, ARRAYSIZE(text), text);
    if (SUCCEEDED(hr))
    {
        hr = spellChecker->Check(text, &enumSpellingError);
    }

    if (SUCCEEDED(hr))
    {
        hr = PrintSpellingErrors(spellChecker, text, enumSpellingError);
        enumSpellingError->Release();
    }
    return hr;
}

// Dit is een routine die ik (SB) heb toegevoegd. 
// Het leest een file in.txt in, checkt alle woorden daarin en schrijft een file out.txt weg. 
// De file in.txt moet staan in de working directory. Deze moet per regel een woord bevatten en in UTF8 zijn gecodeerd.
// Als de file out.txt al bestaat breekt de routine af. 
inline HRESULT CheckFilesCommand(_In_ ISpellChecker* spellChecker)
{
	wchar_t text[MAX_PATH];		// bevat het woord om te checken (UTF-16)
	unsigned char text_utf8[MAX_PATH]; // bevat het woord om te checken (UTF-8)
	HRESULT hr = S_OK;

	FILE *stream_in, *stream_out;
	errno_t err;
	INT32 nwoorden = 0, nfout = 0, ngoed = 0;

	// eerst de outputfile proberen te openen
	err = fopen_s(&stream_out, "out.txt", "w");
	if (err != 0)
	{
		wprintf(L"File out.txt could not be opened\n");
		return (HRESULT) err;
	}

	err = fopen_s(&stream_in, "in.txt", "r");
	if (err != 0)
	{
		wprintf(L"File in.txt could not be opened\n");
		fclose(stream_out);
		return (HRESULT)err;
	}

	fgets((char *)text_utf8, MAX_PATH - 1, stream_in);  //skip preamble of UTF8 file

	while (!feof(stream_in))
	{
		if (fgets((char *)text_utf8, MAX_PATH - 1, stream_in) > 0)	// get next word
		{
			if (text_utf8[strlen((char *)text_utf8)-1] == '\n')
				text_utf8[strlen((char *)text_utf8)-1] = 0;
			MultiByteToWideChar(CP_UTF8, 0, (LPCCH)text_utf8, -1, text, MAX_PATH); // convert word to UTF16
			//wprintf(text);
			//wprintf(L"\n");
			nwoorden++;
			if (nwoorden%100 ==0)
				wprintf(L".");
			fprintf(stream_out,"%s\t", text_utf8);
			IEnumSpellingError* enumSpellingError = nullptr;

			hr = spellChecker->Check(text, &enumSpellingError); // spellcheck the word

			if (SUCCEEDED(hr))
			{
				//hr = PrintSpellingErrors(spellChecker, text, enumSpellingError);  uitgeschreven in volgende blok
				{
					HRESULT hr = S_OK;
					size_t numErrors = 0;
					while (S_OK == hr)
					{
						ISpellingError* spellingError = nullptr;
						hr = enumSpellingError->Next(&spellingError);
						if (S_OK == hr)
						{
							++numErrors;
							//hr = PrintSpellingError(spellChecker, text, spellingError);
							
							spellingError->Release();
						}
					}

					if (0 == numErrors)
					{
						++ngoed;
						//wprintf(L"No errors.\n\n");
						fprintf(stream_out, "0\n");
					}
					else
					{
						++nfout;
						fprintf(stream_out, "1\n");
					}

					PrintErrorIfFailed(L"PrintSpellingErrors", hr);
					//return (SUCCEEDED(hr) ? S_OK : hr);
				}
				enumSpellingError->Release();
			}
		}
	}
	fprintf(stream_out, "\nwoorden:%i,fout:%i,goed:%i", nwoorden, nfout, ngoed);
	fclose(stream_out);
	fclose(stream_in);
	return (HRESULT)0;
}

inline HRESULT CheckAsYouTypeCommand(_In_ ISpellChecker* spellChecker, _In_ PCWSTR buffer)
{
    wchar_t text[MAX_PATH];
    IEnumSpellingError* enumSpellingError = nullptr;
    HRESULT hr = ReadText(buffer, ARRAYSIZE(text), text);
    if (SUCCEEDED(hr))
    {
        hr = spellChecker->ComprehensiveCheck(text, &enumSpellingError);
    }

    if (SUCCEEDED(hr))
    {
        hr = PrintSpellingErrors(spellChecker, text, enumSpellingError);
        enumSpellingError->Release();
    }
    return hr;
}
