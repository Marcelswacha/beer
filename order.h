#ifndef ORDER_H
#define ORDER_H

#include "recipy.h"

#include <QVector>


class Order
{
public:
    Order();

    void addRecipy(const Recipy& r);
    void clear();
    QString makeOrder() const;

private:
    QVector<RecipyMaltItem> _malts;
    QVector<RecipyHopItem> _hops;
};

#endif // ORDER_H
