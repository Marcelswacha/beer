#ifndef HOPINFO_H
#define HOPINFO_H

#include <QString>
#include <QVector>

struct HopInfo
{
    QString name;
    double alpha;
};

static const QVector<HopInfo> hopInfoDB {
    {HopInfo{"marynka", 8.8}},
    {HopInfo{"lubelski", 4.0}},
    {HopInfo{"hallertauer taurus", 15.8}},
    {HopInfo{"magnum", 11.5}},
    {HopInfo{"sybilla", 6.0}},
    {HopInfo{"target", 9.0}},
};

#endif // HOPINFO_H
