#include "hopwidget.h"

#include "hopinfo.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

HopWidget::HopWidget(QWidget *parent) : QWidget(parent)
{
    auto layout = new QHBoxLayout;

    _nameBox = new QComboBox;

    for (auto hop : hopInfoDB) {
        _nameBox->addItem(hop.name);
    }
    connect(_nameBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onHopChanged(int)));
    layout->addWidget(_nameBox);

    _alphaBox = new QDoubleSpinBox;
    _alphaBox->setRange(0, 100);
    _alphaBox->setSingleStep(0.1);
    _alphaBox->setSuffix("[%]");
    _alphaBox->setValue(hopInfoDB[0].alpha);
    layout->addWidget(_alphaBox);

    _massBox = new QDoubleSpinBox;
    _massBox->setRange(0, 10000);
    _massBox->setSingleStep(1);
    _massBox->setSuffix("[g]");
    layout->addWidget(_massBox);

    _timeBox = new QDoubleSpinBox;
    _timeBox->setRange(0, 240);
    _timeBox->setSingleStep(1);
    _timeBox->setSuffix("[min]");
    layout->addWidget(_timeBox);

    _deleteButton = new QPushButton("-");
    _deleteButton->setMaximumWidth(20);
    layout->addWidget(_deleteButton, Qt::AlignRight);
    connect(_deleteButton, SIGNAL(clicked(bool)), this, SIGNAL(deleteRequested()));

    this->setLayout(layout);
}

QString HopWidget::getName() const
{
    return hopInfoDB[_nameBox->currentIndex()].name;
}

double HopWidget::getAlpha() const
{
    return _alphaBox->value();
}

double HopWidget::getMass() const
{
    return _massBox->value();
}

double HopWidget::getTime() const
{
    return _timeBox->value();
}

void HopWidget::setHop(int i)
{
    _nameBox->setCurrentIndex(i);
    _alphaBox->setValue(hopInfoDB[i].alpha);
}

void HopWidget::onHopChanged(int i)
{
    _alphaBox->setValue(hopInfoDB[i].alpha);
}
