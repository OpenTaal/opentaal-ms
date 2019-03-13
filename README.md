    sudo apt-get install virtualbox


https://developer.microsoft.com/en-us/microsoft-edge/tools/vms/

Virtual machine
MSEdge on Win10 (x86) Stable (17.17134)
IE11 on Win8110 (x86)

Select platform
VirtualBox

90 days Passw0rd

    unzip MSEdge.Win10.VirtualBox.zip
    rm -f MSEdge.Win10.VirtualBox.zip
    VBoxManage import 'MSEdge - Win10.ova'

    unzip IE11.Win81.VirtualBox.zip
    rm -f IE11.Win81.VirtualBox.zip
    VBoxManage import 'IE11 - Win81.ova'




stored in ~/'VirtualBox VMs'/ and ~/.config/VirtualBox/


    VBoxManage list vms
    VBoxManage startvm 'MSEdge - Win10'
    VBoxManage showvminfo 'MSEdge - Win10'

    VBoxManage unregistervm --delete 'MSEdge - Win10'



Choose `File` and choose `Import Applience...` or press `(Ctrl+I)`.

Choose `MSEdge - Win10.ova`

Choose `Import`

Choose `Settings` for the imported applience and fix all warnings, otherwise the vm will not start properly or will freeze your device.


add settings






Microsoft Visual C++ 2010 Express 10

https://microsoft-visual-cpp-express.soft32.com/


https://code.visualstudio.com


get focus (mouse and keyboard) outside of vm, press Ctrl+Alt+Del

# Install Git

https://git-scm.com/download/win
