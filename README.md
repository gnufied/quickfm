quickfm
=======

a file manager implemented via qtquick

    BUILD INSTRUCTSIONS

Dependencies: qt >= 5.4

To create makefile execute "qmake".

Then run "make".

    INSTALL

Dist-files: qml files, icons, readme, license and desktop file.

To install run "sudo make install".

    DEFAULTS:

qml files and icons will be install into /usr/share/quickfm.

Desktop file and its icon go to its standart location.

Binary go to /usr/bin.

If you want to tweak paths feel free to edit quickfm.pro

Please select stable branch not master.Code is public domain. 

NOTICE: Icons by kde-breeze(LGPL). Breeze team holds its rights.

    KNOWN ISSUES:
    
-None

    TODO:

-When FileDialog fixed at upstream(KDE side) add it as file picker.