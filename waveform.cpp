/*
Wave form diagram for oto editor
*/

#include "waveform.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

waveform::waveform(QString filename, QWidget *parent):
QWidget(parent)
{
    setGeometry(0,0,620,100);
    setMinimumSize(620,100);
    setMaximumSize(1280,100);
    setBaseSize(620,100);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    f = new QAudioFormat();
    f->setSampleType(QAudioFormat::Float);
    f->setChannelCount(1);
    f->setSampleRate(44100);
    f->setSampleSize(32);
    f->setCodec("audio/x-wav");

    s = new QAudioDecoder(this);
    s->setNotifyInterval(100);
    //connect(s,SIGNAL(sourceChanged()),s,SLOT(changeWav());
    connect(s,SIGNAL(bufferReady()),this,SLOT(drawWave()));
    s->setAudioFormat(*f);
    s->setSourceFilename(filename);
    update();
}
void waveform::paintEvent(QPaintEvent *)
{
    wBrush.setColor(QColor(255,255,255));
    wPen.setColor(Qt::white);
    wPainter.begin(this);
    wPainter.setPen(wPen);
    wPainter.setBrush(wBrush);
    wPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    wPainter.drawRect(0,0,width(),100);
    wPainter.fillRect(0,0,width(),100,Qt::white);

    wPen.setColor(QColor(0,0,255));
    wPen.setStyle(Qt::SolidLine);
    wPen.setWidth(1);
    wBrush.setColor(QColor(0,0,255));

    //if(!(s->sourceFilename().isEmpty()))
    //{
        wPainter.setPen(wPen);
        wPainter.setBrush(wBrush);
        int jump = 0;
        for(int cnt = 0; cnt < length;cnt++)
        {
            if(jump>=waveA.size())
            {
                break;
            }
            wPainter.drawLine(cnt,(float)50+(waveA[cnt])*((float)pow(10,20)),cnt,(float)50);
            //jump+= (int)(15);
            std::cout << cnt <<"\n";
        //}
        }
    wPainter.end();
}
void waveform::drawWave()
{
    if(s->bufferAvailable())
    {

    //disconnect(s,SIGNAL(bufferReady()),this,SLOT(drawWave()));
    QAudioBuffer wB(s->read());

    s->stop();
    length = wB.frameCount();
    std::cout<<length<<"\n";
    double *waveform;
    waveA.resize(length);
    waveA.reserve(length);
    waveform=wB.data<double>();
    for (int i=0;i<length;i++)
    {
        waveA[i]=waveform[i];
        //std::cout<< waveform[i]<<"\n";
    }


    //std::cout << s->errorString().toStdString()<< "\n";
    //disconnect(s,SIGNAL(bufferReady()),this,SLOT(drawWave()));
    //s->stop();
    //blockSignals(true);
    //disconnect(s,SIGNAL(bufferAvailableChanged(bool)),this,SLOT(drawWave()));
    update();
    }
}
void waveform::changeWav(QString &wav)
{
    //connect(s,SIGNAL(bufferReady()),this,SLOT(drawWave()));
    s->setNotifyInterval(100);
    s->setAudioFormat(*f);
    s->setSourceFilename(wav);
    s->sourceFilename()=wav;

    std::cout<< s->sourceFilename().toStdString() <<" " <<s->bufferAvailable() << std::endl;
    s->start();
}
