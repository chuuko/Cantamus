/*
Cantamus
The caffeine-free singing synthesizer for the rest of us
Linux-native singing synthesis system, intended to be an unique system.
Built using the Qt5 framework, without Java, Flash or the like.
NOTE:
This project is only the frontend for Cantamus. Requires libcantamus and stksamp to be fully functional.
TODO:
Initializing functions, actions from the UI.
Drag & drop note moving / "restless system"
Undo/redo system
Finding a default voicebank
Setting up Cantamus for a build system other than qmake
Licensed under the MIT license.
Coding, graphics and cookies by Squeekers.
This product is gluten-free.
No animals or Nanami models were harmed during the creation of this piece of software.
*/
#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationDisplayName("Cantamus");
    a.setApplicationName("Cantamus");
    MainWindow w;
    w.show();

    if(argv[1]!=NULL)
    {
        //char ff = *argv[1];
        w.clickedUst = argv[1];
        w.open();

    }

    return a.exec();
}
