#include "notecontainer.h"

NoteContainer::NoteContainer(QWidget *parent):
    QListWidget(parent), noteLength()
{
    setDragEnabled(true);
    setAcceptDrops(true);
}

void NoteContainer::addNote()
{

}
void NoteContainer::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}
void NoteContainer::dragLeaveEvent(QDragLeaveEvent *event)
{

}
