#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QDialog>
#include <QLabel>
#include <QScrollArea>
#include <QTextStream>
#include <QToolButton>
#include <QComboBox>
#include <QLineEdit>
#include <QtMultimedia>
#include <QFormLayout>
#include <QDesktopWidget>
#include <QListWidget>
#include <QCheckBox>
#include <QFileSystemWatcher>
#include <QDir>
#include <handleust.h>
#include "editorplace.h"
#include "helpbrowser.h"
#include "pianoroll.h"
#include "ui_mainwindow.h"
//extern pianoRoll *idunno;



namespace Ui {
    class QMainWindow;
    class QWindow;
    class QAction;
    class QDialog;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QAction *actionAbout;
    QAction *showHelp;
    QAction *configureVoices;
    QAction *showEditor;
    QAction *settings;
    QAction *lipsync;
    QAction *tostart;
    QAction *rewind;
    QAction *play;
    QAction *pause;
    QAction *stop;
    QAction *fastforward;
    QAction *toend;
    QAction *selectnotes;
    QAction *draw;
    QAction *deletenotes;
    QAction *genFreq;
    QAction *inputNotes;
    QAction *newFile;
    QAction *openFile;
    QAction *saveFile;
    QAction *globalSettings;
    QAction *lyricChanger;
    QAction *voiceIcon;
    QAction *exportProject;
    QToolButton *addNotes;
    QToolButton *changeLyrics;
    QLabel *resampler;
    QLineEdit *resamplerLocation;
    QString resamplerPath;
    QScrollArea *noteEditor;
    QTextStream *sample;
    QToolBar *tool;
    QGridLayout *areaLayout;
    QMenuBar *mainMenuBar;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *tempMenu;
    QMenu *helpMenu;
    QMenu *viewMenu;
    QFrame *editBar;
    QGridLayout *editLayout;
    QGraphicsWidget *noteContainer;
    QLabel *tempo;
    QLabel *quantize;
    QLabel *length;
    QLabel *lyricLabel;
    QComboBox *setQuantize;
    QStatusBar *mainStatus;
    QTabWidget *propEditor;
    QListWidget *propPlace;
    QLineEdit *lineTempo;
    QLineEdit *typeYourTextHere;
    QString voiceLib;
    QMediaPlayer *cAudioPlayer;
    QMediaPlaylist *cAudioStream;
    const char* cHome = getenv("HOME");
    QDir *rootDir;
    int aVb;
    QDir voiceRoot;
    QStringList vbs;
    QComboBox *vbSelect;
    QString vName;
    QFile voiceConf;
    QStringList vbD;
    QLabel *aVBank;
    QStringList Sm;
    QLabel *expName;
    QLineEdit *expPath;
    QPushButton *expSel;
    editorPlace *otoTab;
    QCheckBox *isFormant;
    int foff;

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        Ui::MainWindow *ui;
        QDialog *aboutWindow;
        QComboBox *setLength;
        QString clickedUst;
        void renderProject(QString res);
        int noWinDec;




    public slots:
    //Slots don't have suffixes or what
        void showAboutWindow();
        void showHelpBrowser();
        void switchVoiceEdit();
        void switchNoteEdit();
        void generateFreq();
        void selectPushed();
        void drawPushed();
        void deletePushed();
        void noteGenerate();
        void noteGenerate2();
        void makeNewFile();
        void open();
        void save();
        void switchSettings();
        void showGlobalSettings();
        void setNoteInputLength();
        void updateLyricsBox();
        void updateQuantize();
        void preventMinusGlitch();
        void changeLyricsSlot();
        void showVoiceInfo();
        void updateHor();
        void renderFile();
        void playProject();
        void pauseProject();
        void stopProject();
        void changeVBIndex(int i);
        void listSamples();
        void chooseExp();
        void noFormant(bool f);
    //It should have a note selector, editor and lyrics editor function
    protected:
        void moveEvent(QMoveEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
    private:
        void initializeMenu();
        void initializeActions();
        void initializeWidgets();
        void initializeStatusBar();

};

#endif // MAINWINDOW_H
