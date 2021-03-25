#ifndef GLOBALS_H
#define GLOBALS_H
#include <QtWidgets>
#include <QFileDevice>
#include <QTextStream>
class globalsEditor : public QMainWindow
{
    Q_OBJECT
    QFileDialog dialog;
    QFileDialog pathset;
    QFile globalsConfFile;
    QLabel *dataLabel;
    QLabel *defaultResamplerLabel;
    QLineEdit *dataPlace;
    QLineEdit *resamplerPlace;
    QPushButton *dataButton;
    QPushButton *resamplerButton;
    QPushButton *okButton;
    QString globalString;
    QString resamplerLine;
    QString dataLine;
    QAction *selectsResampler;
    int cl = 0;
    const char *gHome = getenv("HOME");

    public:
        explicit globalsEditor(QWidget *parent=0);
        ~globalsEditor();
        void loadSettings();


    public slots:

        void chooseResampler();
        void setRoot();
        void saveSettings();
        void updateFields();
};
#endif // GLOBALS_H
