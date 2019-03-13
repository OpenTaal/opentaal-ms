# OpenTaal test of Microsoft spell checking

This repository has code te test word lists with the Microsoft spell checker.


## 1 Virtual machine

If you don not have access to a machine with Microsoft, you can run an official
and free virtual image that is only valid for 90 days.


### 1.1 Linux with Virtual Box

For Linux, do the following steps to install and configure Virtual Box.

    sudo apt-get install virtualbox-qt

Go to https://developer.microsoft.com/en-us/microsoft-edge/tools/vms/ and
download for `Virtual machine` the image called `IE11 on Win8110 (x86)` and
choose under `Select platform` the option `VirtualBox`. This will download about
4 GB.

Choose **only** the image `MSEdge on Win10 (x86) Stable (17.17134)` for a device
with eight or more cores and 8 GB or more RAM (swap excluded). Otherwise, the
device will most likely overload and the virtual image will extremely slowly.
Using version 8.1 will use much less resources and is therefore quicker to use.

Do the following to get the file `IE11 - Win81.ova`.

    unzip IE11.Win81.VirtualBox.zip
    rm -f IE11.Win81.VirtualBox.zip

Importing on command line via `VBoxManage` is not practical as optimizations are
proposed via Virtual Box' GUI. Do the following.

    virtualbox

Choose `File` and choose `Import Applience...` or press `(Ctrl+I)` and choose
the file `IE11 - Win81.ova`.

Only after importing has completed, go to `Machine` and choose `Settings...` or
press `(Ctrl+S)`. Several invalid settings will be reported. These are better
viewed as essential optimizations. These are most likely:
* `General`
    * `Advanced` set `Shared Clipboard` to `Host To Guest`.
* `System`
    * `Motherboard` set `Base Memory` to maximum green without triggering a
notification. (Probably 1915 MB is you have 4 GB RAM.)
    * `Processor` set `Processor(s)` to maximum green without triggering a
notification. (Probably 2 CPUs if you have a quad core.)
* `Display`
    * `Screen` set `Video Memory` to half way max that is still green without triggering
notification. (Probably 64 from 128 MB.)
    * `Screen` set `Enable 2D Video Acceleration` to true.
    * `Remote Display` set `Enable Server` to false.

The virtual machine can now be started with `Normal Start` under `Machine` in
`Start`. This will take a moment to completely start.

The username is `IEUser` and the password is `Passw0rd!` for this virtual image.
To escape with mouse and keyboard from the virtual image, press `Ctrl+Alt+Del`.

The virtual image is stopped with `RightCtrl+h`. To toggle full screen	, use
`RightCtrl+F`.

To later on remove all virtual machines or to start fresh, see
`~/'VirtualBox VMs'/` (20 GB) and `~/.config/VirtualBox/`.


## 2 Git

Install Git from https://git-scm.com/download/win and run `Git Bash`. Then
configure git with your gitHub credentials in this way.

    git config --global user.name "GithubUsername"
    git config --global user.email "email@example.com"

Remain in the defailt directory, which is your user home directory, and clone
the repository.

    https://github.com/OpenTaal/opentaal-ms.git


## 3 Visual Studio

Install Visual Studion from https://code.visualstudio.com and subsequently
install the default `C/C++ extension`. Note that Visual Studio also supports
Git, so it is good to have already cloned the repository.

**TODO** Perhaps remove first `.vscode/*` and `opentaal-ms.code-workspace` and
start again.

**TODO** Create project file, build project and run project.

**TODO** Rewrite text below.

### Spell Checking Client Sample

This sample demonstrates how to use the spell checking API as a client

### Sample Language Implementations


C++

### Files:

SampleSpellingClient.sln
SampleSpellingClient.vcxproj
SampleSpellingClient.vcxproj.filters
commands.h
onspellcheckerchanged.h
spellprint.h
util.h
resources.rc
SampleSpellingClient.cpp

NOTE: If using VS11, this sample requires a non-Express version

### To build the sample using the command prompt:

    1. Open the Command Prompt window and navigate to the Client directory.
    2. Type msbuild SampleSpellingClient.sln.


### To build the sample using Visual Studio (preferred method):

    1. Open File Explorer and navigate to the Client directory.
    2. Double-click the icon for the SampleSpellingClient.sln file to open the file in Visual Studio.
    3. In the Build menu, select Build Solution. The application will be built in the default \Debug or \Release directory.

### To install the sample:

    1. Navigate to the directory that contains the new executable, using the command prompt or File Explorer.
    2. Create a folder called "SampleSpellingClient" under "Program Files".
    3. Copy SampleSpellingClient.exe to Program Files\SampleSpellingClient.


## 4 Tests

**TODO** Test with word lists.

**TODO** Rewrite text below.

### To run the sample:

    Just run through the command line. The possibilities are:

    - SampleSpellingClient.exe: with no arguments, it will list all languages for which there is an available spell checker in the system
    - SampleSpellingClient.exe <language tag>: it will start an interactive spell checking session using the default provider for the language (if there's more than one, you can change it in the Control Panel)

    Session example:
    > SampleSpellingClient.exe en-us
    Language: en-us

    Provider: MsSpell (Microsoft Windows Spellchecker)
    
    Options:
    
    Commands:
    quit - Quit
    add <word> - Add word
    ac <word> <word> - Add autocorrect pair
    ign <word> - Ignore word
    chk <text> - Check text
    lst <wordlist> - List words in specified wordlist (0 = ignored words, 1 = added
    words, 2 = excluded words, 3 = autocorrect pairs)
    > chk helo
    helo [0, 3] is misspelled. Suggestions:
             hello
             halo
             helot
             help
             hell
             held
             hellos
             hero
             helm
    
    > add helo
    Spell checker changed.
    > chk helo
    No errors.
    
    > chk washington
    washington [0, 9] is misspelled. It should be autocorrected to:
            Washington
    
    > chk helo helo
    helo [5, 8] is misspelled. It should be deleted.
    
    > ign washington
    Spell checker changed.
    > chk washington
    No errors.
    
    > chk tre
    tre [0, 2] is misspelled. Suggestions:
             tree
             trek
             trey
             true
             tare
             tire
             tore
             rte.
             tee
             try
    
    > ac tre tree
    > Spell checker changed.
    chk tre
    tre [0, 2] is misspelled. It should be autocorrected to:
            tree
    
    > lst 0
    washington
    > lst 1
    helo
    > lst 2
    > lst 3
    tre     tree
    > quit
