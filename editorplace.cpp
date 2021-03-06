#include "editorplace.h"
#include <iostream>

/*
Configuration editor. Currently supports only UTAU-compatible OTO files*/

editorPlace::editorPlace(QWidget *parent) :
    QWidget(parent)
{
    otoInfo = new QLabel ("Edit your voicebank configuration here.\nClick on 'Load' to open an oto.ini file");
    otoTable = new QTableWidget(this);
    otoHeaders << "File" << "Alias" << "Sample start" << "Sample fixed" << "Sample cutoff" << "Consonant slide" << "Overlap";
    otoButton = new QPushButton("Load oto.ini",this);
    QFont font;
    font.setFamily("japanese");

    otoTable->setFont(font);

    otoTable->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    otoTable->setSizeIncrement(2,2);
    otoTable->setColumnCount(7);
    otoTable->setHorizontalHeaderLabels(otoHeaders);

    connect(otoTable,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(updateWave(int,int,int,int)));

    eLayout = new QGridLayout(this);
    eLayout->addWidget(otoInfo,1,1);
    eLayout->addWidget(otoTable,2,1);
    eLayout->setRowMinimumHeight(3,100);
    eLayout->addWidget(otoButton,4,1);
    setLayout(eLayout);
}
void editorPlace::loadOto(QString otoFile)
{
    QString lineO;
    QTextCodec *dec= QTextCodec::codecForName("Shift_JIS");
    QFile *o=new QFile(otoFile+"/oto.ini");
    aVoice = otoFile;
    o->open(QFile::ReadOnly);
    QString dec2((dec->toUnicode(o->readAll())));
    QTextStream otos(&dec2);
    otos.setAutoDetectUnicode(true);
    int otoEntry = 0;
    while(otos.atEnd()==0)
    {

        lineO = otos.readLine();
        int f = lineO.indexOf(QString("="));
        if(f!=-1)
        {
            otoTable->setRowCount(otoEntry+1);
            otoV.resize(otoEntry+1);
            otoV.reserve(otoEntry+1);
            otoTable->setCellWidget(otoEntry,0,new QLabel(lineO.mid(0,f)));
            otoV[otoEntry]=lineO.mid(0,f);
            int a = lineO.indexOf(QString(","),f+1);
            otoTable->setCellWidget(otoEntry,1,new QLabel(lineO.mid(f+1,a-f-1)));
            int of = lineO.indexOf(QString(","),a+1);
            otoTable->setCellWidget(otoEntry,2,new QLabel(lineO.mid(a+1,of-a-1)));
            int c = lineO.indexOf(QString(","),of+1);
            otoTable->setCellWidget(otoEntry,3,new QLabel(lineO.mid(of+1,c-of-1)));
            int cu = lineO.indexOf(QString(","),c+1);
            otoTable->setCellWidget(otoEntry,4,new QLabel(lineO.mid(c+1,cu-c-1)));
            int p = lineO.indexOf(QString(","),cu+1);
            otoTable->setCellWidget(otoEntry,5,new QLabel(lineO.mid(cu+1,p-cu-1)));
            otoTable->setCellWidget(otoEntry,6,new QLabel(lineO.mid(p+1)));
            otoEntry++;
        }
        waveWidget = new waveform(aVoice+"/"+otoV[0],this);
        eLayout->addWidget(waveWidget,3,1);
        //std::cout<<f<<" "<<a<<" "<<of<<" "<<c<<" "<<cu<<" "<<p<< " "<<ov<<" "<<"\n";
    }
}
void editorPlace::updateWave(int x,int y,int sx,int sy)
{
    std::cout<<otoV[x].toStdString()<<"\n";
    QString wFName = aVoice+"/"+otoV[x];
    waveWidget->changeWav(wFName);
}
