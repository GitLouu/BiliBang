#ifndef BANG_H
#define BANG_H
#include <QString>
#include <QVector>

struct Bang
{
    QString title;
    QString cover;
    QString pubIndex;
    QString pubTime;
    int seasonId;
    QString url;
};

struct Bangumi
{
    QVector<Bang*> bangs;
    QString date;
    int dayOfWeek;
};

struct DFile
{
    QString url;
    QString fname;
};

#endif // BANG_H
