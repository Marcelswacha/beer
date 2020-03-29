#include "mainwindow.h"

#include "hopinfo.h"
#include "hopwidget.h"
#include "maltwidget.h"
#include "maltinfo.h"
#include "order.h"
#include "recipy.h"

#include <algorithm>

#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>

#include <QVector>
#include <QPair>
#include <QDebug>

static const int NUM_MALT_TYPES = 3;
static const int NUM_HOP_TYPES = 2;

static double density(double blg)
{
    static QVector<QPair<double, double>> densityMap {
        {10., 1.040},
        {11., 1.044},
        {12., 1.048},
        {13., 1.053},
        {14., 1.057},
        {15., 1.061},
        {16., 1.065},
        {17., 1.070},
        {18., 1.074},
        {19., 1.078},
        {20., 1.083}
    };

    for (int i = 1; i < densityMap.size(); ++i) {
        double first = densityMap[i-1].first;
        double second = densityMap[i].first;
        double firstValue = densityMap[i-1].second;
        double secondValue = densityMap[i].second;

        if (blg == first) {
            return firstValue;
        }
        else if (blg == second) {
            return secondValue;
        }
        else if (first < blg && second > blg) {
            double p = (blg - first) / (second - first);
            return firstValue + p * (secondValue - firstValue);
        }
    }

    return -1;
}

struct HopUsageArray
{
    double a, b, c;
};

double hopUsage(double time, double blg)
{
    static QVector<QPair<double, HopUsageArray>> hopMap {
        {0.,  {0.0,  0.0, 0.0}},
        {5.,  {5.0,  4.8, 4.3}},
        {10., {12.0, 11.4, 10.4}},
        {20., {15.0, 14.3, 13.0}},
        {30., {19.0, 18.1, 16.5}},
        {45., {22.0, 21.0, 19.1}},
        {60., {24.0, 22.9, 20.9}},
        {75., {27.0, 25.7, 23.5}}
    };

    auto interp1D = [&](double targetX, double x1, double x2, double y1, double y2) ->double {
      double p = (targetX - x1) / (x2 - x1);
      return y1 + p * (y2 - y1);
    };

    if (time > 75.) {
        time = 75.;
    }

    blg = std::max(12.5, blg);
    blg = std::min(20.0, blg);

    for (int i = 1; i < hopMap.size(); ++i) {
        double firstTime = hopMap[i-1].first;
        double secondTime = hopMap[i].first;

        if (time >= firstTime && time <= secondTime) {
            double tl, tr, dl, dr, x1, x2;
            if (blg < 15) {
                tl = hopMap[i-1].second.a;
                tr = hopMap[i-1].second.b;
                dl = hopMap[i].second.a;
                dr = hopMap[i].second.b;
                x1 = 12.5;
                x2 = 15.0;
            }
            else {
                tl = hopMap[i-1].second.b;
                tr = hopMap[i-1].second.c;
                dl = hopMap[i].second.b;
                dr = hopMap[i].second.c;
                x1 = 15.0;
                x2 = 20.0;
            }

            double interpBlgLow = interp1D(blg, x1, x2, tl, tr);
            double interpBlgHigh = interp1D(blg, x1, x2, dl, dr);

            double result = interp1D(time, firstTime, secondTime, interpBlgLow, interpBlgHigh);
            return result;
        }
    }

    return -1;
}

void MainWindow::compute()
{
    // malts
    // total mass
    const double blg = _blg->value();
    double totalVolume = _volume->value() + _volumeLoss->value();
    double mass = totalVolume * density(blg);

    double sumContrib = 0.0;
    for (auto w : _maltWidgets) {
        sumContrib += w->getContrib();
    }
    qDebug() << "Total percent: " << sumContrib;

    // extract per 1 kg
    double extractPerKilo = 0;
    double eff = _efficiency->value() / 100.;
    for (auto w : _maltWidgets) {
        extractPerKilo += w->getContrib() * w->getMaltEff();
    }
    extractPerKilo *= eff;

    double extractTotal = mass * (_blg->value() / 100.);
    double kilos = extractTotal / extractPerKilo;

    for (auto w : _maltWidgets) {
        w->setMass(kilos * w->getContrib());
    }

    // hops
    auto computeEBU = [&](double blg, double vol, double alpha, double time, double mass) {
        return mass * alpha * hopUsage(time, blg) / (vol * 10);
    };

    double totalEBU = 0;
    for (auto w : _hopWidgets) {
        double ebu = computeEBU(blg, totalVolume, w->getAlpha(), w->getTime(), w->getMass());
        totalEBU += ebu;
    }

    _ebu->setText(QString::number(totalEBU));
}

void MainWindow::onAddMalt()
{
    auto w = new MaltWidget;
    _maltLayout->addWidget(w);
    _maltWidgets.push_back(w);
    connect(w, SIGNAL(deleteRequested()), this, SLOT(onDeleteMalt()));
}

void MainWindow::onAddHop()
{
    auto w = new HopWidget;
    _hopLayout->addWidget(w);
    _hopWidgets.push_back(w);
    connect(w, SIGNAL(deleteRequested()), this, SLOT(onDeleteHop()));
}

void MainWindow::onDeleteMalt()
{
    MaltWidget* maltWidget = qobject_cast<MaltWidget*>(sender());
    _maltLayout->removeWidget(maltWidget);
    _maltWidgets.removeOne(maltWidget);
    delete maltWidget;
}

void MainWindow::onDeleteHop()
{
    HopWidget* hopWidget = qobject_cast<HopWidget*>(sender());
    _hopLayout->removeWidget(hopWidget);
    _hopWidgets.removeOne(hopWidget);
    delete hopWidget;
}

void MainWindow::addRecipy()
{
    Recipy recipy;
    recipy.setInfo(_volume->value(), _blg->value(), _ebu->text().toDouble());
    for(auto w : _maltWidgets) {
        double mass = w->getMass();
        if (mass > 0.) {
            recipy.addMalt(w->getName(), w->getMass());
        }
    }
    for (auto w : _hopWidgets) {
        recipy.addHop(w->getName(), w->getMass(), w->getTime());
    }

    _recipies.push_back(recipy);
    printRecipies();
}

void MainWindow::clearRecipies()
{
    _textEdit->clear();
    _recipies.clear();
}

void MainWindow::printRecipies()
{
    _textEdit->clear();
    int i = 1;
    Order order;
    for (auto r : _recipies) {
        _textEdit->append(QString::number(i++) + QString("\n"));
        _textEdit->append(r.getRecipy());
        order.addRecipy(r);
    }

    _textEdit->append(order.makeOrder());


}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto mainWidget = new QWidget;
    auto mainLayout = new QHBoxLayout;

    /********************Malts***************/
    _maltLayout = new QVBoxLayout;
    auto maltBox = new QGroupBox;
    maltBox->setTitle("Słód");

    auto volLabel = new QLabel("Objętość do rozlewu");
    _volume = new QDoubleSpinBox;
    _volume->setValue(20.);

    auto volLossLabel = new QLabel("Straty przy rozlewie");
    _volumeLoss = new QDoubleSpinBox;
    _volumeLoss->setValue(2.);

    auto blgLabel = new QLabel("BLG");
    _blg = new QDoubleSpinBox;
    _blg->setValue(13.);
    _blg->setRange(10, 20);

    auto effLabel = new QLabel("Wydajność ważelni");
    _efficiency = new QDoubleSpinBox;
    _efficiency->setRange(1, 100);
    _efficiency->setValue(75);

    _ebu = new QLabel("0");

    auto maltParamsLayout = new QGridLayout;
    maltParamsLayout->addWidget(volLabel, 0, 0);
    maltParamsLayout->addWidget(_volume, 0, 1);
    maltParamsLayout->addWidget(volLossLabel, 1, 0);
    maltParamsLayout->addWidget(_volumeLoss, 1, 1);
    maltParamsLayout->addWidget(blgLabel, 2, 0);
    maltParamsLayout->addWidget(_blg, 2, 1);
    maltParamsLayout->addWidget(effLabel, 3, 0);
    maltParamsLayout->addWidget(_efficiency, 3, 1);
    maltParamsLayout->addWidget(new QLabel("EBU: "), 4, 0);
    maltParamsLayout->addWidget(_ebu, 4, 1);
    _maltLayout->addLayout(maltParamsLayout);

    _addMaltButton = new QPushButton("+");
    _addMaltButton->setMaximumWidth(20);
    connect(_addMaltButton, SIGNAL(clicked()), this, SLOT(onAddMalt()));
    _maltLayout->addWidget(_addMaltButton, Qt::AlignRight);

    for (int i=0; i < NUM_MALT_TYPES; ++i) {
        auto maltWidget = new MaltWidget;
        _maltWidgets.push_back(maltWidget);
        _maltLayout->addWidget(maltWidget);
        connect(maltWidget, SIGNAL(deleteRequested()), this, SLOT(onDeleteMalt()));
        maltWidget->setMalt(i);
    }
    maltBox->setLayout(_maltLayout);
    mainLayout->addWidget(maltBox);

    /********************HOPS**************/
    _hopLayout = new QVBoxLayout;
    auto hopBox = new QGroupBox;
    hopBox->setTitle("Chmiel");

    _addHopButton = new QPushButton("+");
    _addHopButton->setMaximumWidth(20);
    connect(_addHopButton, SIGNAL(clicked()), this, SLOT(onAddHop()));
    _hopLayout->addWidget(_addHopButton, Qt::AlignRight);

    for (int i=0; i < NUM_HOP_TYPES; ++i) {
        auto hopWidget = new HopWidget;
        _hopWidgets.push_back(hopWidget);
        _hopLayout->addWidget(hopWidget);
        connect(hopWidget, SIGNAL(deleteRequested()), this, SLOT(onDeleteHop()));
        hopWidget->setHop(i);
    }
    hopBox->setLayout(_hopLayout);
    mainLayout->addWidget(hopBox);



    /**************Buttons*************/
    auto buttonsBox = new QGroupBox("");
    auto buttonsLayout = new QVBoxLayout;

    _computeButton = new QPushButton("Oblicz");
    connect(_computeButton, SIGNAL(clicked()), this, SLOT(compute()));
    buttonsLayout->addWidget(_computeButton);

    auto addButton = new QPushButton("Dodaj przepis");
    connect(addButton, SIGNAL(clicked(bool)), this, SLOT(addRecipy()));
    buttonsLayout->addWidget(addButton);

    auto clearButton = new QPushButton("Usuń recepty");
    connect(clearButton, SIGNAL(clicked(bool)), this, SLOT(clearRecipies()));
    buttonsLayout->addWidget(clearButton);

    buttonsBox->setLayout(buttonsLayout);
    mainLayout->addWidget(buttonsBox);

    /************Recipies**************/
    _textEdit = new QTextEdit;
    mainLayout->addWidget(_textEdit);



    mainWidget->setLayout(mainLayout);
    this->setCentralWidget(mainWidget);
}

MainWindow::~MainWindow()
{

}
