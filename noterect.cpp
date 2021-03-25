#include "noterect.h"
#include <QtWidgets>

/*
Note geometry for piano roll
*/

QRectF giveToPaint;
int intToPaint;
QString lirToPaint;

noteRect::noteRect(QString noteRectLyrics, int noteRectLength, int noteRectPitch, int &noteRectPos, QWidget *parent)
    :QLabel(parent)
{
    QRectF constructorRect=QRectF(noteRectPos,(2640-(noteRectPitch*20)-240),(noteRectLength/4),20);
    giveToPaint=QRectF(constructorRect);
    intToPaint=noteRectLength/4;
    lirToPaint=noteRectLyrics;
    setText(lirToPaint);
}
void noteRect::paintEvent(QPaintEvent *)
{
/*
    QImage rectPix(intToPaint,20,QImage::Format_ARGB32_Premultiplied);
    //rectPix.fill(qRgba(255,154,203,255));
   // QString textShown(noteRectLyrics);
    QPen pen(Qt::SolidLine);
    pen.setColor(QColor(0,0,0,255));
    QBrush brush(Qt::SolidPattern);
    brush.setColor(QColor(255,154,203,255));
    QPainter painter;
    painter.begin(&rectPix);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(giveToPaint);
    painter.fillRect(giveToPaint,QColor(255,154,203,255));
    painter.drawText(giveToPaint,QString(lirToPaint));
    painter.end();
    setPixmap(QPixmap::fromImage(rectPix));*/

}
