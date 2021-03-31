#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pianoroll.h"
#include "editorplace.h"
#include "notecontainer.h"
#include <QGridLayout>
#include <QImage>
#include <QLabel>
#include <QGraphicsWidget>
#include <QPicture>
#include <QGraphicsView>
#include <QScrollBar>
#include <QGraphicsGridLayout>
#include <QPixmap>
#include <QFileDialog>
#include <iostream>
#include <handleust.h>
#include "globals.h"

int f;
QString lirToMake("");
QVector<QString> lirToMake2;
int lirCount=1;
int lirPos=0;
int lirNoteCount;
int noteTracker=0;
int prevAct;
int anotherPos=0;


/*
    ustHandler needs libcantamus to function.
*/

ustHandler *handler = new ustHandler();
pianoRoll *pRoll;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    foff=1;
    rootDir = new QDir(QString(cHome) + "/.cantamus");

    if (QDir(rootDir->canonicalPath() + "/data/voicebank").exists()==false)
        rootDir->mkpath(QString(cHome) + "/.cantamus/data/voicebank");
    if (QDir(rootDir->canonicalPath() + "/data/lipsync").exists()==false)
        rootDir->mkpath(QString(cHome) + "/.cantamus/data/lipsync");

    QFile globalsConf(QString(cHome) + "/.cantamus/globals");

    if (QFile(QString(rootDir->canonicalPath()) + "/globals").exists()==false)
    {

        globalsConf.open(QFile::WriteOnly | QFile::Text);
        QTextStream gStream (&globalsConf);
        gStream << "RESAMPLER_GLOBAL=/usr/bin/stksamp" << endl << "ROOT=" << cHome << "/.cantamus" << endl;
        resamplerPath = QString("/usr/bin/stksamp");
        globalsConf.close();
    }
    else
    {
        globalsConf.open(QFile::ReadOnly | QFile::Text);
        QTextStream gStream (&globalsConf);
        if(gStream.readLine().contains("RESAMPLER_GLOBAL="))
        {
            gStream.seek(0);
            resamplerPath=gStream.readLine().mid(17);
        }
    }


    cAudioPlayer=new QMediaPlayer;
    connect(cAudioPlayer,SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    cAudioPlayer->setVolume(50);

    cAudioStream = new QMediaPlaylist;


    voiceRoot = QString(cHome) + "/.cantamus/data/voicebank";

    voiceRoot.setSearchPaths("vbs",QStringList(voiceRoot.absolutePath()));

    vbs = voiceRoot.entryList();
    //vbs.removeAt(1);
    //vbs.removeAt(2);

    aVb = 1;



    for(int la = 2;la < vbs.length(); la++)
    {
        voiceLib = voiceRoot.absolutePath() + "/" + vbs.at(la);

        if(vbs.length()==0)
        {
            QMessageBox::warning(this,"Error","There are no voicebanks. Please install a voicebank first");
        }

        voiceConf.setFileName(voiceLib + QString("/bank.conf"));
        voiceConf.open(QFile::ReadOnly);
        if(voiceConf.isOpen()==false)
        {
            voiceConf.setFileName(voiceLib + QString("/character.txt"));
            voiceConf.open(QFile::ReadOnly);
            if(voiceConf.isOpen()==false)
            {
                QMessageBox::warning(this,"Error","No bank.conf found");
                break;
            }
        }

        QTextStream vData(&voiceConf);

        vName = vData.readLine();
        vName.remove(0,5);
        vbD.insert(la-1,vName);
    }

    voiceLib = voiceRoot.absolutePath() + "/" + vbs.at(aVb+2);

    listSamples();

    cAudioPlayer->setPlaylist(cAudioStream);

    lirToMake2.resize(10000);
    ui->setupUi(this);

    setWindowTitle("Cantamus");

    initializeActions();
    initializeWidgets();
    initializeMenu();
    setMenuBar(mainMenuBar);
    setMouseTracking(true);

    switchNoteEdit();
    initializeStatusBar();



    /*Qt Creator tends to append semicolons regardless of place in code */
    /*Also, add slots from source code to the UI with Change Slots*/

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeActions()
{
    actionAbout = new QAction (ui->actionAbout);
    actionAbout->setText("About");
    showHelp = new QAction (ui->actionShow_help);
    showHelp->setText("Show help");
    configureVoices = new QAction (ui->actionVoice_configuration);
    configureVoices->setText("Voicebank configuration");
    showEditor = new QAction(ui->actionEditor);
    showEditor->setText("Note editor");
    settings = new QAction (ui->actionProject_settings);
    settings->setText("Project settings");
    genFreq = new QAction(this);
    genFreq->setText("Generate freqmaps");
    selectnotes= new QAction(ui->actionSelect);
    draw = new QAction(ui->actionDraw_notes);
    deletenotes=new QAction(ui->actionErase);
    newFile=new QAction(ui->actionNew);
    newFile->setText("New");
    openFile=new QAction(this);
    openFile->setText("Open");
    saveFile=new QAction(this);
    saveFile->setText("Save");
    globalSettings = new QAction(this);
    globalSettings->setText("Global settings");
    voiceIcon = new QAction(ui->action_voiceIcon);
    play = new QAction(ui->actionPlay);
    pause = new QAction(ui->actionPause);
    stop = new QAction(ui->actionStop);
    exportProject = new QAction(this);
    exportProject->setText("Export project");


    connect(actionAbout,SIGNAL(triggered()),this,SLOT(showAboutWindow()));
    connect(showHelp,SIGNAL(triggered()),this,SLOT(showHelpBrowser()));
    connect(configureVoices,SIGNAL(triggered()),this,SLOT(switchVoiceEdit()));
    connect(showEditor,SIGNAL(triggered()),this,SLOT(switchNoteEdit()));
    connect(genFreq,SIGNAL(triggered()),this,SLOT(generateFreq()));
    connect(openFile,SIGNAL(triggered()),this,SLOT(open()));
    connect(saveFile,SIGNAL(triggered()),this,SLOT(save()));
    connect(settings,SIGNAL(triggered()),this,SLOT(switchSettings()));
    connect(globalSettings,SIGNAL(triggered()),this,SLOT(showGlobalSettings()));
    connect(voiceIcon,SIGNAL(triggered()),this,SLOT(showVoiceInfo()));
    connect(play,SIGNAL(triggered()),this,SLOT(playProject()));
    connect(pause,SIGNAL(triggered()),this,SLOT(pauseProject()));
    connect(stop,SIGNAL(triggered()),this,SLOT(stopProject()));
    connect(exportProject,SIGNAL(triggered()),this,SLOT(renderFile()));


}

void MainWindow::showAboutWindow()
{
    aboutWindow = new QDialog(this);
    aboutWindow -> show();
    aboutWindow -> setWindowTitle("Cantamus - About");
    QLabel *versionString = new QLabel("Cantamus Experimental Build");
    QLabel *appIcon = new QLabel(this);
    QPixmap *appIconI = new QPixmap("://images/icons/cantamus.svg");
    appIcon->setPixmap(*appIconI);
    QGridLayout *aboutLayout = new QGridLayout(this);
    aboutWindow-> setLayout(aboutLayout);
    aboutLayout->setSpacing(1);
    aboutLayout->setColumnStretch(1,2);
    aboutLayout->setRowStretch(1,2);
    aboutLayout->addWidget(appIcon,1,1,Qt::AlignCenter);
    aboutLayout->addWidget(versionString,3,1);
    aboutWindow -> activateWindow();
}
void MainWindow::initializeStatusBar()
{
    mainStatus=new QStatusBar(ui->statusBar);
    mainStatus->showMessage("Ready");
}
void MainWindow::initializeMenu()
{
    mainMenuBar = new QMenuBar(ui->menuBar);
    fileMenu = new QMenu(ui->menuFile);
    editMenu = new QMenu(ui->menuEdit);
    viewMenu = new QMenu(ui->menuView);
    tempMenu = new QMenu(ui->menuTemporary);
    helpMenu = new QMenu(ui->menuHelp);

    fileMenu= mainMenuBar->addMenu(tr("File"));
    fileMenu->addAction(newFile);
    fileMenu->addAction(openFile);
    fileMenu->addAction(saveFile);
    editMenu = mainMenuBar->addMenu(tr("Edit"));
    viewMenu = mainMenuBar->addMenu(tr("View"));

    viewMenu->addAction(showEditor);
    viewMenu->addAction(configureVoices);

    tempMenu = mainMenuBar->addMenu(tr("Temporary"));
    tempMenu->addAction(genFreq);
    tempMenu->addAction(exportProject);
    tempMenu->addAction(globalSettings);
    helpMenu = mainMenuBar->addMenu(tr("Help"));
    helpMenu->addAction(showHelp);
    helpMenu->addAction(actionAbout);


}

void MainWindow::showVoiceInfo()
{
    if (voiceConf.isOpen()==false)
    {
        QMessageBox::warning(this,"Voicebank information", "There are no voicebanks. Please install a voicebank first.");
        return;
    }
    else
    {
        QDialog *vBox;
        vBox = new QDialog(this);
        vBox->setGeometry(100,100,300,100);
        vBox->setMinimumSize(200,100);
        vBox->setMaximumSize(400,100);
        QGridLayout *vLayout;
        vLayout = new QGridLayout(this);
        QLabel *vIcon;
        vIcon = new QLabel(this);
        vIcon->setGeometry(0,0,150,150);
        if(QFile(voiceLib + "/image.bmp").exists()==false)
        {
            vIcon->setPixmap(QPixmap("://images/icons/singer.svg"));
        }
        else
        {
            vIcon->setPixmap(QPixmap(voiceLib+"/image.bmp"));
        }
        QLabel *vLabel;
        vLabel = new QLabel(this);
        vLabel->setText(QString("Name: " + vbD.at(aVb)));
        vLayout->addWidget(vIcon,0,0);
        vLayout->addWidget(vLabel,0,1);
        vBox->setLayout(vLayout);

        //vBox.setIcon("://images/icons/singer.svg");
        vBox->setWindowTitle("Voicebank information");
        vBox->show();
        //QMessageBox::warning(this,"Voicebank information", vName);

    }
}

void MainWindow::initializeWidgets()
{

    tool = new QToolBar(ui->mainToolBar);


    otoTab = new editorPlace(this);

    editBar = new QFrame(this);
    editBar->setFrameStyle(QFrame::Raised);
    editBar->setFrameShadow(QFrame::Raised);
    editBar->setFrameShape(QFrame::StyledPanel);
    editBar->setGeometry(0,0,640,40);
    editBar->setMinimumSize(640,40);
    editBar->setBaseSize(640,40);
    editBar->setMaximumSize(1280,40);
    editBar->setSizeIncrement(640,0);
    editBar->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);

    QGridLayout *editBarLayout = new QGridLayout(this);

    tempo = new QLabel("Tempo");
    quantize = new QLabel("Quantize");
    length = new QLabel("Length");
    lyricLabel = new QLabel("Lyrics");

    setQuantize = new QComboBox(this);
    setQuantize->insertItem(0,"Full note");
    setQuantize->insertItem(1,"1/2 note");
    setQuantize->insertItem(2,"1/3 note");
    setQuantize->insertItem(3,"1/4 note");
    setQuantize->insertItem(4,"1/6 note");
    setQuantize->insertItem(5,"1/8 note");
    setQuantize->insertItem(6,"1/12 note");
    setQuantize->insertItem(7,"1/16 note");
    setQuantize->insertItem(8,"1/24 note");
    setQuantize->insertItem(9,"1/32 note");
    setQuantize->insertItem(10,"1/48 note");
    setQuantize->insertItem(11,"1/64 note");
    setQuantize->setCurrentIndex(3);
    connect(setQuantize,SIGNAL(currentIndexChanged(int)),this,SLOT(updateQuantize()));

    setLength = new QComboBox(this);
    setLength->insertItem(0,"Full note");
    setLength->insertItem(1,"1/2 note");
    setLength->insertItem(2,"1/3 note");
    setLength->insertItem(3,"1/4 note");
    setLength->insertItem(4,"1/6 note");
    setLength->insertItem(5,"1/8 note");
    setLength->insertItem(6,"1/12 note");
    setLength->insertItem(7,"1/16 note");
    setLength->insertItem(8,"1/24 note");
    setLength->insertItem(9,"1/32 note");
    setLength->insertItem(10,"1/48 note");
    setLength->insertItem(11,"1/64 note");
    setLength->setCurrentIndex(3);
    connect(setLength,SIGNAL(currentIndexChanged(int)),this,SLOT(setNoteInputLength()));

    double t;
    t=120;
    QString tempoValue=QString::number(t);

    //How come I can't use pointers as arguments?

    lineTempo = new QLineEdit(this);
    lineTempo->setGeometry(0,0,50,20);
    lineTempo->setMinimumSize(50,20);
    lineTempo->setBaseSize(50,20);
    lineTempo->setMaximumSize(50,20);
    lineTempo->setText(tempoValue);

    typeYourTextHere = new QLineEdit(this);

    connect(typeYourTextHere,SIGNAL(textChanged(QString)),this,SLOT(updateLyricsBox()));


    inputNotes = new QAction(this);
    addNotes = new QToolButton(this);
    inputNotes->setIcon(QIcon("://images/icons/addnotes.svg"));
    addNotes->setDefaultAction(inputNotes);

    connect(inputNotes,SIGNAL(triggered()),this,SLOT(noteGenerate()));
    lyricChanger=new QAction(this);
    lyricChanger->setIcon(QIcon("://images/icons/changelyrics.svg"));
    changeLyrics = new QToolButton(this);
    changeLyrics->setIcon(QIcon("://images/icons/changelyrics.svg"));
    changeLyrics->setDefaultAction(lyricChanger);
    connect(lyricChanger,SIGNAL(triggered()),this,SLOT(changeLyricsSlot()));

    editBarLayout->addWidget(tempo,0,0);
    editBarLayout->addWidget(lineTempo,0,1);
    editBarLayout->addWidget(quantize,0,2);
    editBarLayout->addWidget(setQuantize,0,3);
    editBarLayout->addWidget(length,0,4);
    editBarLayout->addWidget(setLength,0,5);
    editBarLayout->addWidget(lyricLabel,0,6);
    editBarLayout->addWidget(typeYourTextHere,0,7);
    editBarLayout->addWidget(addNotes,0,8);
    editBarLayout->addWidget(changeLyrics,0,9);


    pRoll = new pianoRoll;
    pRoll->show();

    connect(selectnotes,SIGNAL(triggered()),this,SLOT(selectPushed()));
    //emit selectnotes->triggered();

    connect(draw,SIGNAL(triggered()),this,SLOT(drawPushed()));
    //emit draw->triggered();

    connect(deletenotes,SIGNAL(triggered()),this,SLOT(deletePushed()));
    connect(newFile,SIGNAL(triggered()),this,SLOT(makeNewFile()));

    //For zooming on the piano roll

    QSize pianoSize = pRoll->size();

    noteEditor = new QScrollArea(this);
    noteEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    noteEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    noteEditor->setBaseSize(640,341);
    noteEditor->setMinimumSize(640,341);
    noteEditor->setMaximumSize(1280,853);
    noteEditor->setSizeIncrement(640,512);
    noteEditor->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    noteEditor->setWidget(pRoll);
    noteEditor->setWidgetResizable(true);
    noteEditor->verticalScrollBar()->setSliderPosition(1450);
    //pRoll->newScroll =0;
    pRoll->oldScroll =1450;
    updateGeometry();
    pRoll->scrollCor_x = 0;
    pRoll->winPos= y();
    updateHor();
    //pRoll->winPos_old =  pRoll->winPos;

    connect(noteEditor->horizontalScrollBar(),SIGNAL(sliderMoved(int)),this,SLOT(preventMinusGlitch()));
    connect(noteEditor->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(updateHor()));

    editBar->setLayout(editBarLayout);

    resampler = new QLabel("Resampler:");
    resamplerLocation = new QLineEdit(this);

    expName = new QLabel(".wav file name:");
    expPath = new QLineEdit(this);
    expSel = new QPushButton("Browse...",this);

    aVBank = new QLabel("Voicebank:");

    vbSelect = new QComboBox(this);
    vbSelect->addItems(vbD);
    vbSelect->setCurrentIndex(1);

    connect(vbSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(changeVBIndex(int)));
    connect(expSel,SIGNAL(released()),this,SLOT(chooseExp()));
    //vbSelect->removeItem(0);
   // vbSelect->removeItem(1);


    otoTab->setMinimumSize(110,20);
    otoTab->loadOto((voiceLib+QString("/oto.ini")));

    propEditor = new QTabWidget(this);
    propEditor->setGeometry(0,0,640,47);
    propEditor->setMinimumSize(640,47);
    propEditor->setMaximumSize(1280,47);

    propPlace = new QListWidget(propEditor);

    propEditor->addTab(propPlace,"Volume");

    isFormant = new QCheckBox(this);
    isFormant->setText("No formant filter");
    connect(isFormant,SIGNAL(toggled(bool)),this,SLOT(noFormant(bool)));

    areaLayout = new QGridLayout(this);
    areaLayout->addWidget(otoTab,0,0);
    areaLayout->addWidget(editBar,0,0);
    areaLayout->addWidget(noteEditor,1,0);
    areaLayout->addWidget(propEditor,2,0);
    areaLayout->addWidget(expName,0,0);
    areaLayout->addWidget(expPath,0,1);
    areaLayout->addWidget(expSel,0,2);
    areaLayout->addWidget(resampler,1,0);
    areaLayout->addWidget(resamplerLocation,1,1);
    areaLayout->addWidget(aVBank,2,0);
    areaLayout->addWidget(vbSelect,2,1);
    areaLayout->addWidget(isFormant,3,0);
    areaLayout->setContentsMargins(0,0,0,0);



    ui->centralWidget->setLayout(areaLayout);

    setNoteInputLength();

    updateQuantize();

}
void MainWindow::changeVBIndex(int i)
{
    std::cout << i << "\n";
    aVb = i;
    voiceLib = voiceRoot.absolutePath() + "/" + vbs.at(aVb+2);
    Sm.clear();
    listSamples();
    otoTab->loadOto(voiceLib);
}
void MainWindow::updateHor()
{


    updateGeometry();
    noteEditor->verticalScrollBar()->updateGeometry();
    pRoll->scrollCor = 1450+y();
    pRoll->newScroll = noteEditor->verticalScrollBar()->sliderPosition()-pRoll->oldScroll;
    pRoll->noteLengths = 0;


}
void MainWindow::moveEvent(QMoveEvent *event)
{
    if(pRoll->winPos==0)
    {
        pRoll->winPos = menuBar()->y();
        pRoll->winPos_old = pRoll->winPos;
    }

    pRoll->winPosx = x();
    pRoll->winPos = y();
    updateHor();

}


void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        pRoll->winPos = y();
    }

}

void MainWindow::switchVoiceEdit()
{
    otoTab->setHidden(false);
    editBar->setHidden(true);
    noteEditor->setHidden(true);
    propEditor->setHidden(true);
    expName->setHidden(true);
    expPath->setHidden(true);
    expSel->setHidden(true);
    resampler->setHidden(true);
    resamplerLocation->setHidden(true);
    aVBank->setHidden(true);
    vbSelect->setHidden(true);
    isFormant->setHidden(true);
}

void MainWindow::switchNoteEdit()
{
    otoTab->setHidden(true);
    editBar->setHidden(false);
    noteEditor->setHidden(false);
    propEditor->setHidden(false);
    expName->setHidden(true);
    expPath->setHidden(true);
    expSel->setHidden(true);
    resampler->setHidden(true);
    resamplerLocation->setHidden(true);
    aVBank->setHidden(true);
    vbSelect->setHidden(true);
    isFormant->setHidden(true);
}
void MainWindow::switchSettings()
{
    otoTab->setHidden(true);
    editBar->setHidden(true);
    noteEditor->setHidden(true);
    propEditor->setHidden(true);
    expName->setHidden(false);
    expPath->setHidden(false);
    expSel->setHidden(false);
    resampler->setHidden(false);
    resamplerLocation->setHidden(false);
    aVBank->setHidden(false);
    vbSelect->setHidden(false);
    isFormant->setHidden(false);
}

void MainWindow::listSamples()
{
    QDir sD = voiceLib;
    QStringList fl;
    fl.append("*.wav");
    Sm = sD.entryList(fl);
    std::cout << Sm.length() << "\n";
}
void MainWindow::generateFreq()
{


    for (int cn=0;cn<Sm.length();cn++)
    {
        std::string cmd ="stksamp --genfreq ";
        //QMessageBox::warning(this,"",cmd.c_str());
        std::string vl = voiceLib.toStdString();
        //QMessageBox::warning(this,"",vl.data());
        cmd.append(vl.c_str());
        //QMessageBox::warning(this,"",cmd.c_str());
        cmd.append("/");
        //QMessageBox::warning(this,"",cmd.c_str());
        std::string cm = Sm.at(cn).toStdString();
        cmd.append(cm.c_str());
        //QMessageBox::warning(this,"",cmd.c_str());
        system(cmd.c_str());
        cmd.clear();
    }
    QMessageBox::warning(this,"Cantamus","Freqmaps genereated");
}

void MainWindow::showHelpBrowser()
{
    helpBrowser *helpB = new helpBrowser(this);
    helpB->activateWindow();
    helpB->show();
}

void MainWindow::selectPushed()
{
    pRoll->selectMode();
    //connect(selectnotes,SIGNAL(triggered()),pRoll,SLOT(selectMode()));
    //emit selectnotes->triggered();
}

void MainWindow::drawPushed()
{
    pRoll->drawMode();
    //connect(selectnotes,SIGNAL(triggered()),pRoll,SLOT(selectMode()));
    //emit draw->triggered();
}
void MainWindow::deletePushed()
{
    pRoll->eraseMode();
}
void MainWindow::noteGenerate()
{


    if (setLength->currentIndex()==0)
    {
        pRoll->inputNoteLength=1920;
    }
    else if (setLength->currentIndex()==1)
    {
        pRoll->inputNoteLength=960;
    }
    else if (setLength->currentIndex()==2)
    {
        pRoll->inputNoteLength=640;
    }
    else if (setLength->currentIndex()==3)
    {
        pRoll->inputNoteLength=480;
    }
    else if (setLength->currentIndex()==4)
    {
        pRoll->inputNoteLength=320;
    }
    else if (setLength->currentIndex()==5)
    {
        pRoll->inputNoteLength=240;
    }
    else if (setLength->currentIndex()==6)
    {
        pRoll->inputNoteLength=160;
    }
    else if (setLength->currentIndex()==7)
    {
        pRoll->inputNoteLength=120;
    }
    else if (setLength->currentIndex()==8)
    {
        pRoll->inputNoteLength=80;
    }
    else if (setLength->currentIndex()==9)
    {
        pRoll->inputNoteLength=60;
    }
    else if (setLength->currentIndex()==10)
    {
        pRoll->inputNoteLength=40;
    }
    else
    {
        pRoll->inputNoteLength=30;
    }

    QString lirString(typeYourTextHere->text());
    QTextStream lirStream(&lirString);
    QString lirString2(lirStream.readAll());

    lirToMake.resize(0);
    lirNoteCount=pRoll->actualnotecount;
    prevAct=pRoll->actualnotecount;
    int noteLCount = lirString.size();


    for(int strcount=0; strcount<noteLCount;strcount++)
    {
        if(lirString2.mid(lirCount,1)==QString(","))
        {

            lirToMake=lirString2.mid(lirPos,lirCount-lirPos);

            if(lirCount==lirString2.size())
            {
                lirToMake2[lirNoteCount]=lirToMake.mid(0,lirToMake.size()-1);
            }

            else
            {
                lirToMake2[lirNoteCount]=lirToMake;
                lirPos=lirCount+1;
            }
            lirNoteCount++;
        }
        else
        {
            lirToMake.insert(lirCount,lirString2.mid(lirCount,1));
        }

        lirCount = lirCount+1;
    }

    lirCount = 1;
    lirPos = 0;
    noteGenerate2();
}
void MainWindow::noteGenerate2()
{


    pRoll->actualnotecount=lirNoteCount;
    for (f=noteTracker;f<lirNoteCount+1;f++)
    {
        if(f>=prevAct)
        {
            pRoll->actualnotelength[f]=pRoll->inputNoteLength;
            pRoll->actualnotelyrics[f]=lirToMake2[f];
            pRoll->actualnotepitch[f]=37;
        }
    }

    f=0;
    noteTracker=pRoll->actualnotecount;

    pRoll->update();



    //lirNoteCount=lirNoteCount+1;
    lirNoteCount=0;
    lirToMake.clear();
    lirToMake.resize(0);
    lirToMake2.clear();
    lirToMake2.resize(10000);
}
void MainWindow::makeNewFile()
{
    pRoll->clearRoll();
    handler->ustNoteCount=0;
    //handler->closeFile(0);
    //handler->file.close();
    noteTracker=0;

}
void MainWindow::open()
{
    makeNewFile();
    QString ustFile;


if(clickedUst.startsWith("/"))
    {
       ustFile=clickedUst;
    }

    else
    {
        ustFile=QFileDialog::getOpenFileName(this,"Open file",cHome,"UTAU sheets (*.ust);;Cantamus snippets (*.cvs)");
    }

    if (!ustFile.isEmpty())
    {  handler->ustLoader((const std::string) ustFile.toStdString());

    //handler->ustNoteLength[0]=120;
    std::cout<<"a"<<std::endl;

    pRoll->actualnotecount=handler->ustNoteCount;

    std::cout<<"a"<<std::endl;

    for (f=0;f<handler->ustNoteCount+1;f++)
    {
        pRoll->actualnotelength[f]=handler->ustNoteLength[f];
        std::cout<<handler->ustNoteLyric[f]<<" "<<handler->isRest[f]<<std::endl;
        pRoll->actualnotelyrics[f]=QString::fromStdString(handler->ustNoteLyric[f]);
        pRoll->actualnotepitch[f]=handler->ustNotePos[f];
        pRoll->lstore=pRoll->lstore+pRoll->actualnotelength[f];
        pRoll->actNote=f;

        /*if (f == lim)
        {
            pRoll->fromOpen = true;
            pRoll->evaluate();
            lim = lim + (handler->ustNoteCount/90);
        }*/

        if(handler->isRest[f]==0)

            pRoll->makeRest[f] = 0;

        else
            pRoll->makeRest[f] = 1;

    }
    pRoll->fromOpen = true;
    pRoll->evaluate();
    pRoll->afterOpen = true;
    pRoll->evaluate();
    f=0;
    //std::cout << handler->ustNoteCount<< endl;
    //pRoll->setMinimumWidth(handler->ustNoteCount*100);
    //pRoll->evaluate();
    pRoll->update();
    QDialog *dialog=new QDialog;
    QLabel *noteShow = new QLabel(this);

    mainStatus->showMessage("Opened "+ustFile,0);
    //mainStatus->showMessage("Ready");

    noteShow->setText("The length of the first note is "+QString::number(handler->ustNoteLength[0])+" "+QString::number(pRoll->actualnotelength[0]));
    QGridLayout *nLayout = new QGridLayout(this);
    nLayout->addWidget(noteShow,0,0);
    dialog->setLayout(nLayout);
    //dialog->show();
    }
}
void MainWindow::save()
{
    QString saveFile;

    saveFile=QFileDialog::getSaveFileName(this,"Save file",QString(cHome) + "/.cvs;;/.ust","Cantamus snippets (*.cvs);;UTAU Script Text (*ust)");
    if (!saveFile.isEmpty())
    {    handler->ustNoteCount=pRoll->actualnotecount;

    for (f=0;f<pRoll->actualnotecount+1;f++)
    {
        if(pRoll->makeRest[f]==true)
        {
            handler->isRest[f]=0;
        }
        else
        {
            handler->isRest[f]=1;
        }
        handler->ustNoteLength[f]=pRoll->actualnotelength[f];
        handler->ustNoteLyric[f]=pRoll->actualnotelyrics[f].toStdString();
        handler->ustNotePos[f]=pRoll->actualnotepitch[f];

    }

    f=0;
    handler->printToCVS(saveFile.toStdString());
    mainStatus->showMessage("Saved to "+saveFile,10000);
    mainStatus->showMessage("Ready");}
}
void MainWindow::chooseExp()
{
    QString expFn=QFileDialog::getSaveFileName(this,"Specify path...",QString(cHome)+"/.wav","WAV files (*.wav)");
    expPath->setText(expFn);
    expPath->update();
}
void MainWindow::renderFile()
{
    if(expPath->text().isEmpty()==false)
    {
        renderProject(expPath->text());
    }
}
void MainWindow::renderProject(QString res)
{
    QString rCvs = resamplerPath +" "+ QString::number(foff)+" " + voiceLib+" " + QString::fromStdString(handler->ustOFile)+" " + res;

    std::cout <<rCvs.toStdString().c_str()<<endl;
    if(handler->ustOFile=="")
    {
        QMessageBox::warning(this,"Error","Please save the project first");
    }
    else
    {
        system(rCvs.toStdString().c_str());
        mainStatus->showMessage("Rendered file");
    }
}
void MainWindow::playProject()
{
    cAudioStream->clear();
    QString rWav = "rm "+ rootDir->path()+"/tmp.wav";
    renderProject(rootDir->path() +"/tmp.wav");
    if(QFile(rootDir->path()+"/tmp.wav").exists()==false)
    {
        QMessageBox::warning(this,"Error","No output produced");
    }
    else
    {
        cAudioStream->addMedia(QMediaContent(QUrl::fromLocalFile(rootDir->path()+"/tmp.wav")));
        cAudioPlayer->play();
        system(rWav.toStdString().c_str());
    }
    
    //QMessageBox::warning(this,"",QString::number(handler->ustNoteCount));
    /*for(int s=0;s<pRoll->actualnotecount;s++)
    {*/

    //}

}

void MainWindow::pauseProject()
{
    cAudioPlayer->pause();
}

void MainWindow::stopProject()
{
    cAudioPlayer->stop();
}

void MainWindow::showGlobalSettings()
{
    globalsEditor *confGlobal = new globalsEditor(this);
    confGlobal->activateWindow();
    confGlobal->show();
}
void MainWindow::setNoteInputLength()
{
    if (setLength->currentIndex()==0)
    {
        pRoll->inputNoteLength=1920;
    }
    else if (setLength->currentIndex()==1)
    {
        pRoll->inputNoteLength=960;
    }
    else if (setLength->currentIndex()==2)
    {
        pRoll->inputNoteLength=640;
    }
    else if (setLength->currentIndex()==3)
    {
        pRoll->inputNoteLength=480;
    }
    else if (setLength->currentIndex()==4)
    {
        pRoll->inputNoteLength=320;
    }
    else if (setLength->currentIndex()==5)
    {
        pRoll->inputNoteLength=240;
    }
    else if (setLength->currentIndex()==6)
    {
        pRoll->inputNoteLength=160;
    }
    else if (setLength->currentIndex()==7)
    {
        pRoll->inputNoteLength=120;
    }
    else if (setLength->currentIndex()==8)
    {
        pRoll->inputNoteLength=80;
    }
    else if (setLength->currentIndex()==9)
    {
        pRoll->inputNoteLength=60;
    }
    else if (setLength->currentIndex()==10)
    {
        pRoll->inputNoteLength=40;
    }
    else
    {
        pRoll->inputNoteLength=30;
    }

}
void MainWindow::updateLyricsBox()
{
    typeYourTextHere->text()=typeYourTextHere->text();
    pRoll->lyricRouter=typeYourTextHere->text();

}

void MainWindow::updateQuantize()
{
    if (setQuantize->currentIndex()==0)
        {
            pRoll->quantizeValue=1920;
        }
        else if (setQuantize->currentIndex()==1)
        {
            pRoll->quantizeValue=960;
        }
        else if (setQuantize->currentIndex()==2)
        {
            pRoll->quantizeValue=640;
        }
        else if (setQuantize->currentIndex()==3)
        {
            pRoll->quantizeValue=480;
        }
        else if (setQuantize->currentIndex()==4)
        {
            pRoll->quantizeValue=320;
        }
        else if (setQuantize->currentIndex()==5)
        {
            pRoll->quantizeValue=240;
        }
        else if (setQuantize->currentIndex()==6)
        {
            pRoll->quantizeValue=160;
        }
        else if (setQuantize->currentIndex()==7)
        {
            pRoll->quantizeValue=120;
        }
        else if (setQuantize->currentIndex()==8)
        {
            pRoll->quantizeValue=80;
        }
        else if (setQuantize->currentIndex()==9)
        {
            pRoll->quantizeValue=60;
        }
        else if (setQuantize->currentIndex()==10)
        {
            pRoll->quantizeValue=40;
        }
        else
        {
            pRoll->quantizeValue=30;
        }
}
void MainWindow::preventMinusGlitch()
{
    updateQuantize();
    setNoteInputLength();
    pRoll->scrollAmount=noteEditor->horizontalScrollBar()->x();
    pRoll->scrollCor_x=noteEditor->horizontalScrollBar()->sliderPosition();
    pRoll->noteLengths = 0;
    //pRoll->changePrecedingNoteTrackIfNeeded=noteEditor->horizontalScrollBar()->pageStep();
}
void MainWindow::changeLyricsSlot()
{
    //pRoll->lirTransfer=pRoll->lyricInput;

    pRoll->finishLirChange2();

}
void MainWindow::noFormant(bool f)
{
    if(f==1)
    {
        foff=0;
    }
    else
    {
        foff=1;
    }
    std::cout  <<foff <<"\n";
}
