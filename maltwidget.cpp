#include "maltwidget.h"

#include "maltinfo.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

MaltWidget::MaltWidget(QWidget *parent) : QWidget(parent)
{
    auto layout = new QHBoxLayout;

    _nameBox = new QComboBox;

    for (auto malt : maltInfoDB) {
        _nameBox->addItem(malt.name);
    }
    layout->addWidget(_nameBox);

    _contribBox = new QDoubleSpinBox;
    _contribBox->setRange(0, 100);
    _contribBox->setSingleStep(1);
    _contribBox->setSuffix("[%]");
    layout->addWidget(_contribBox);

    _massLabel = new QLabel("0 [kg]");
    layout->addWidget(_massLabel);

    _deleteButton = new QPushButton("-");
    _deleteButton->setMaximumWidth(20);
    layout->addWidget(_deleteButton, Qt::AlignRight);
    connect(_deleteButton, SIGNAL(clicked(bool)), this, SIGNAL(deleteRequested()));

    this->setLayout(layout);
}

QString MaltWidget::getName() const
{
    return maltInfoDB[_nameBox->currentIndex()].name;
}

double MaltWidget::getContrib() const
{
    return _contribBox->value() / 100.;
}

double MaltWidget::getMaltEff() const
{
    return maltInfoDB[_nameBox->currentIndex()].extract;
}

double MaltWidget::getMass() const
{
    return _mass;
}

void MaltWidget::setMass(double mass)
{
    _mass = mass;
  _massLabel->setText(QString::number(mass) + " [kg]");
}

void MaltWidget::setMalt(int i)
{
    _nameBox->setCurrentIndex(i);
}
