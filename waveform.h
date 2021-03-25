#ifndef WAVEFORM_H
#define WAVEFORM_H
#include <QGraphicsWidget>
#include <QPaintDevice>
#include <QWidget>
#include <QtMultimedia>
#include <QPaintEvent>
#include <QPainter>
#include <QObject>

class waveform : public QWidget
{
    Q_OBJECT

public:
    explicit waveform(QString filename, QWidget *parent =0);
    QAudioFormat *f;
    //QPaintDevice wP;
    QPainter wPainter;
    QPen wPen;
    QBrush wBrush;
    int length;
    QVector<qreal> waveA;
    void changeWav(QString &wav);
public slots:
    void drawWave();
protected:
    void paintEvent(QPaintEvent *);
private:
     QAudioDecoder *s;
};
#endif // WAVEFORM_H
