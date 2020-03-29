#ifndef MALTINFO_H
#define MALTINFO_H

#include <QString>
#include <QVector>

struct MaltInfo
{
    QString name;
    double extract;
};

static const QVector<MaltInfo> maltInfoDB {
    {MaltInfo{"pilzneński", 0.8}},
    {MaltInfo{"wiedeński", 0.795}},
    {MaltInfo{"monachijski", 0.795}},
    {MaltInfo{"karmelowy jasny", 0.76}},
    {MaltInfo{"karmelowy ciemny", 0.73}},
    {MaltInfo{"pale ale", 0.79}},
    {MaltInfo{"pszeniczny", 0.82}},
    {MaltInfo{"pszeniczny ciemny", 0.82}},
    {MaltInfo{"pszeniczny czekoladowy", 0.75}},
    {MaltInfo{"czekoladowy jasny", 0.68}},
    {MaltInfo{"czekoladowy ciemny", 0.68}},
    {MaltInfo{"kawowy", 0.755}},
    {MaltInfo{"carafa III", 0.65}},
    {MaltInfo{"palony jęczmień", 0.7}},
    {MaltInfo{"płatki jęczmienne", 0.7}},
    {MaltInfo{"płatki żytnie", 0.6}}
};


#endif // MALTINFO_H
