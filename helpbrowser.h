#ifndef HELPBROWSER_H
#define HELPBROWSER_H
#include <QWidget>
#include <QPlainTextEdit>
#include "mainwindow.h"


namespace help
{
    class QMainWindow;
}


class helpBrowser : public QMainWindow
{
  Q_OBJECT
    QGridLayout *helpLayout;
    QLabel *helpLabel;
    QTextDocument *helpDoc;

    public:
        explicit helpBrowser(QWidget *parent=0);
        const char *home = getenv("HOME");
        ~helpBrowser();

    private:
        void initializeContents();
        void open();
        void loadFile();
        QPlainTextEdit *helpBox;
};

#endif // HELPBROWSER_H
