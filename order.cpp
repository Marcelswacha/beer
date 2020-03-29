#include "order.h"

Order::Order()
{

}

void Order::addRecipy(const Recipy &r)
{
    for (auto rmalt : r.malts()) {
        bool found = false;
        for (auto& m : _malts) {
            if (m.name == rmalt.name) {
                m.mass += rmalt.mass;
                found = true;
                break;
            }
        }
        if (!found) {
            _malts.push_back(rmalt);
        }
    }

    for (auto rhop : r.hops()) {
        bool found = false;
        for (auto& h : _hops) {
            if (rhop.name == h.name) {
                h.mass += rhop.mass;
                found = true;
                break;
            }
        }
        if (!found) {
            _hops.push_back(rhop);
        }
    }
}

void Order::clear()
{
    _malts.clear();
    _hops.clear();
}

QString Order::makeOrder() const
{
    QString result;

    result += "Słody wszystkie\n";
    for (auto malt : _malts) {
        result += malt.name + " :\t" + QString::number(malt.mass) + " [kg]\n";
    }

    result += "\nChmiele wszystkie\n";
    for (auto hop : _hops) {
        result += hop.name + " :\t" + QString::number(hop.mass) + " [g]\n";
    }

    result += "\n\n";

    return result;
}
