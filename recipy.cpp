#include "recipy.h"

#include <QTextStream>

Recipy::Recipy()
{

}

void Recipy::setInfo(double volume, double blg, double ebu)
{
    _volume = volume;
    _blg = blg;
    _ebu = ebu;
}

void Recipy::addMalt(QString name, double mass)
{
    _malts.push_back({name, mass});
}

void Recipy::addHop(QString name, double mass, double time)
{
    _hops.push_back({name, mass, time});

    bool found = false;
    for (int i = 0; i < _concatenatedHops.size(); ++i) {
        if (_concatenatedHops[i].name == name) {
            _concatenatedHops[i].mass += mass;
            found = true;
            break;
        }
    }
    if (!found) {
        _concatenatedHops.push_back({name, mass, time});
    }
}

QString Recipy::getRecipy() const
{
    QString result;

    result += QString("Objętość: ") + QString::number(_volume) + " [l]\n";
    result += QString("BLG: ") + QString::number(_blg) + "\n";
    result += QString("EBU: ") + QString::number(_ebu) + "\n\n";

    result += "Słody\n";
    for (auto malt : _malts) {
        if (malt.mass > 0.) {
            result += malt.name + " :\t" + QString::number(malt.mass) + " [kg]\n";
        }
    }

    result += "\nChmiele\n";
    for (auto hop : _hops) {
        if (hop.mass > 0.) {
            result += hop.name + " :\t" + QString::number(hop.mass) + " [g]\t" + QString::number(hop.time) + " [min]\n";
        }
    }

    result += "\n\n";

    return result;
}
