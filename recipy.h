#ifndef RECIPY_H
#define RECIPY_H

#include <QString>
#include <QVector>

struct RecipyMaltItem
{
    QString name;
    double mass;
};

struct RecipyHopItem
{
    QString name;
    double mass;
    double time;
};

class Recipy
{
public:
    Recipy();

    void setInfo(double volume, double blg, double ebu);
    void addMalt(QString name, double mass);
    void addHop(QString name, double mass, double time);

    const QVector<RecipyMaltItem>& malts() const { return _malts; }
    const QVector<RecipyHopItem>& hops() const { return _concatenatedHops; }

    QString getRecipy() const;

private:
    double _volume;
    double _blg;
    double _ebu;

    QVector<RecipyMaltItem> _malts;
    QVector<RecipyHopItem> _hops;

    QVector<RecipyHopItem> _concatenatedHops;

};

#endif // RECIPY_H
