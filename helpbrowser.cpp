#include "helpbrowser.h"

/*
Stupid little help browser
*/

helpBrowser::helpBrowser(QWidget *parent):
    QMainWindow(parent)
{
    setGeometry(0,0,300,300);
    setWindowTitle("Help browser");
    initializeContents();
    setContentsMargins(5,5,5,5);

}
helpBrowser::~helpBrowser()
{

}
void helpBrowser::initializeContents()
{
    helpBox=new QPlainTextEdit;

    setCentralWidget(helpBox);
    open();
}
void helpBrowser::open()
{
    loadFile();
}
void helpBrowser::loadFile()
{
    QFile helpfile("HELP");
    helpfile.setFileName(QString(home) + "/Cantamus/HELP");
    helpfile.open(QFile::ReadOnly | QFile::Text);
    QTextStream helpShow(&helpfile);

    helpBox->setPlainText(helpShow.readAll());
}
