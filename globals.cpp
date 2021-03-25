#include "globals.h"
#include <QGridLayout>
#include <stdio.h>
#include <iostream>

/*
Global settings editor
*/

globalsEditor::globalsEditor(QWidget *parent):
    QMainWindow(parent)
{
    QWidget *gWindow = new QWidget(this);
    setCentralWidget(gWindow);
    setWindowTitle("Global settings");
    setGeometry(0,0,420,200);
    setMinimumSize(420,200);
    setBaseSize(420,200);
    setSizeIncrement(860,824);
    setMaximumSize(1280,1024);
    QGridLayout *glayout = new QGridLayout(this);
    gWindow->setLayout(glayout);
    setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    defaultResamplerLabel = new QLabel(this);
    defaultResamplerLabel->setText("Resampler:");
    defaultResamplerLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    resamplerPlace = new QLineEdit(this);
    resamplerPlace->setGeometry(0,0,150,20);
    resamplerPlace->setMinimumSize(150,20);
    resamplerPlace->setBaseSize(150,20);
    resamplerPlace->setMaximumSize(1110,20);
    resamplerPlace->setSizeIncrement(960,0);
    resamplerPlace->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    dataLabel = new QLabel(this);
    dataLabel->setText("Cantamus root directory:");
    dataPlace = new QLineEdit(this);
    dataPlace->setGeometry(0,0,150,20);
    dataPlace->setMinimumSize(150,20);
    dataPlace->setBaseSize(150,20);
    dataPlace->setMaximumSize(1110,20);
    dataPlace->setSizeIncrement(960,0);
    dataPlace->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

    resamplerButton = new QPushButton(this);
    resamplerButton->setText("Browse...");
    dataButton = new QPushButton(this);
    dataButton->setText("Browse...");
    okButton = new QPushButton(this);
    okButton->setText("OK");



    //glayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

    glayout->addWidget(defaultResamplerLabel,0,0);
    glayout->setSpacing(0);
    glayout->setRowMinimumHeight(0,30);
    glayout->setRowStretch(0,1);
    glayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    glayout->setGeometry(QRect(0,0,320,200));
    //glayout->setColumnStretch(2,3);
    glayout->setColumnMinimumWidth(2,100);
    glayout->setRowMinimumHeight(2,100);
    glayout->addWidget(resamplerPlace,0,1);
    glayout->addWidget(resamplerButton,0,2);
    glayout->addWidget(dataLabel,1,0);
    glayout->addWidget(dataPlace,1,1);
    glayout->addWidget(dataButton,1,2);
    glayout->addWidget(okButton,3,0);
    loadSettings();
    resamplerPlace->setText(resamplerLine);
    dataPlace->setText(dataLine);

    connect(resamplerButton,SIGNAL(clicked()),this,SLOT(chooseResampler()));
    connect(dataButton,SIGNAL(clicked()),this,SLOT(setRoot()));
    connect(okButton,SIGNAL(clicked()),this,SLOT(saveSettings()));
    connect(resamplerPlace,SIGNAL(textEdited(QString)),this,SLOT(updateFields()));
    connect(dataPlace,SIGNAL(textEdited(QString)),this,SLOT(updateFields()));

}
globalsEditor::~globalsEditor()
{}
void globalsEditor::loadSettings()
{
    globalsConfFile.setFileName(QString(gHome) + "/.cantamus/globals");
    if(globalsConfFile.exists()==false)
    {
        resamplerLine = "/usr/bin/cantamus-resampler";
        dataLine = QString(gHome) + "/.cantamus";
        saveSettings();
    }

    globalsConfFile.open(QFile::ReadWrite | QFile::Text);
    QTextStream globalsRead(&globalsConfFile);

    while(!(globalString=globalsRead.readLine()).isNull())
    {
        if(globalString.startsWith("RESAMPLER_GLOBAL="))
            resamplerLine = globalString.mid(17);
        if(globalString.startsWith("ROOT="))
            dataLine = globalString.mid(5);
    }
}
void globalsEditor::saveSettings()
{
    QTextStream globalsWritten(&globalsConfFile);
    globalsConfFile.reset();
    globalsConfFile.resize(0);
    globalsWritten.seek(0);

    globalsWritten << "RESAMPLER_GLOBAL="<<resamplerLine<<"\n"<<"ROOT="<<dataLine;

    std::cout << "Globals saved\n";

    close();
}
void globalsEditor::chooseResampler()
{
    resamplerLine=dialog.getOpenFileName(this);
    resamplerPlace->setText(resamplerLine);
}
void globalsEditor::setRoot()
{
    pathset.setFileMode(QFileDialog::Directory);
    pathset.setOption(QFileDialog::ShowDirsOnly);
    dataLine=pathset.getExistingDirectory(this);
    dataPlace->setText(dataLine);
}
void globalsEditor::updateFields()
{
    resamplerLine = resamplerPlace->text();
    dataLine = dataPlace->text();
}
