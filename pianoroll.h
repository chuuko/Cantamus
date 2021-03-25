#ifndef PIANOROLL_H
#define PIANOROLL_H

#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <QRect>
#include <QPainter>
#include <QDialogButtonBox>
#include <math.h>
#include <handleust.h>
#include "notecontainer.h"
#include "mainwindow.h"

namespace piano
{
    QT_BEGIN_NAMESPACE
    class QGraphicsWidget;
    class QRect;
    QT_END_NAMESPACE
}
class pianoRoll : public QWidget
{
    Q_OBJECT

    QGridLayout *noteLayout;

    int count;
    int noteline;
    int fonty;
    int numy;
    int buttonCount;
    QMenu *rightClickMenu;
    QDialog *properties;
    QAction *actionProp;
    QAction *actionDel;



    public:
        pianoRoll(QWidget *parent=0);
        bool fromOpen;
        bool afterOpen;
        int *noteMode;
        QLabel *wPosL;
        int noteCount;
        bool noteLast;
        bool noteFb;
        int rowCount;
        int pToSend;
        int pToSend2;
        QString lirTransfer;
        int actualnotecount;
        std::vector<int> actualnotelength;
        QVector<QString> actualnotelyrics;
        QVector<int> noteS;
        std::vector<int> actualnotepitch;
        //int selArray[10000];
        int inputNotePitch;
        unsigned int inputNoteLength;
        int p;
        int isFromButton;
        int y();
        int winPos;
        int winPosx;
        int winPos_old;
        int winPosx_old;
        int changePrecedingNoteTrackIfNeeded;
        int lirChangeI;
        int pitchFromPos(int posToConvert);
        int noteSize();
        signed int scrollCor;
        signed int scrollCor_x;
        signed int newScroll;
        signed int oldScroll;
        signed int modCorr;
        signed int modCorr2;
        bool deleteActNote;
        QVector <int> makeRest;
        int u = 0;
        QVector <int> undoCodes;
        QVector <bool> selArray;


        QLabel *lengthLabel2;
        QLabel *lyricLabel2;
        QLineEdit *lengtharea;
        QLineEdit *lyricarea;
        QString propLyric;
        QString dLyric;
        QString dLyric2;
        QString dLyric3;
        QDialogButtonBox propButs;

        int propLength;
        QGridLayout *propLayout;
        void selectRect(QPoint mousePos);
        void growSelect(QPoint growth);
        QPoint corner;
        unsigned int quantizeValue;
        unsigned int scrollAmount;
        unsigned int lstore;
        unsigned int noteLengths;
        int actNote;

        void showMouseMenu(QPoint mousePos);
        QString lyricInput;
        QString lyricRouter;
        void moveEvent(QMoveEvent *event);
        void noteMake();
        void clearRoll();
        void evaluate();
        void changesLength();
        void deleteNote();
        void addUndo(int code);
        int mn;
    public slots:
        void del();
        void selectMode();
        void drawMode();
        void eraseMode();
        void finishLirChange();
        void finishLirChange2();
        void showProperties();
    protected:

        void paintEvent(QPaintEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void dragMoveEvent(QDragMoveEvent *event);
        void mouseDoubleClickEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);

        void dragEnterEvent(QDragEnterEvent *event);
        void dragLeaveEvent(QDragLeaveEvent *event);
        void dropEvent(QDropEvent *event);

    private:
        unsigned int quantizeMultiplier(const unsigned int xToConvert) const;
        QRectF targetRect1 (QPointF position);
        int pitchLineLength;
        unsigned int length1;
};
#endif // PIANOROLL_H
