#ifndef OTOPARSER_H
#define OTOPARSER_H

#include <QtNumeric>
#include <QFileOpenEvent>
#include <QFileDevice>
#include <QTextStream>

//oto.ini handler

namespace oto
{
    QT_BEGIN_NAMESPACE
    class otoParser;
    QT_END_NAMESPACE
}

class otoParser: public QObject
{
    Q_OBJECT

    public:

        QTextStream otoWriter;
        QFile otoFile;
        const QString otoFormat;
        QString otoContents;

        void readOto(const QString &otoFormat);

};

#endif // OTOPARSER_H
