#ifndef NOTECONTAINER_H
#define NOTECONTAINER_H

#include "pianoroll.h"
#include <QWidget>
#include <QListWidget>
namespace notes
{
    class QListWidget;
    class NoteContainer;
}

class NoteContainer : public QListWidget
{
    Q_OBJECT
    QColor cantamusBaseColor;


    public:
        explicit NoteContainer(QWidget *parent=0);
        float noteLength;
        void addNote();

    public slots:

    protected:

        void dragEnterEvent(QDragEnterEvent *event);
        void dragLeaveEvent(QDragLeaveEvent *event);

    private:
};
#endif // NOTECONTAINER_H
