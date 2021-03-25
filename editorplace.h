#ifndef EDITORPLACE_H
#define EDITORPLACE_H
#include "waveform.h"
#include <QScrollArea>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QVector>

class editorPlace : public QWidget
{

    Q_OBJECT

    public:
        explicit editorPlace(QWidget *parent = 0);
        void loadOto(QString otoFile);
        QVector<QString> otoV;

public slots:
        void updateWave(int x, int y, int sx, int sy);
    private:
        QLabel *otoInfo;
        QGridLayout *eLayout;
        QString aVoice;
        QTableWidget* otoTable;
        QStringList otoHeaders;
        QPushButton *otoButton;
        waveform *waveWidget;
};

#endif // EDITORPLACE_H

