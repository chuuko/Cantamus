#ifndef NOTERECT_H
#define NOTERECT_H

#include <QtWidgets>
namespace noteRectN {

    class noteRect;
    class QLabel;
    class QWidget;

}


class noteRect : public QLabel
{
    public:
        noteRect(QString noteRectLyrics, int noteRectLength, int noteRectPitch, int &noteRectPos, QWidget *parent);

    protected:
        void paintEvent(QPaintEvent *event);
};
#endif //NOTERECT_H
