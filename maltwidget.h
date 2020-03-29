#ifndef MALTWIDGET_H
#define MALTWIDGET_H

#include <QWidget>
#include <QString>

class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QPushButton;

class MaltWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MaltWidget(QWidget *parent = 0);

    QString getName() const;
    double getContrib() const;
    double getMaltEff() const;

    double getMass() const;
    void setMass(double mass);


signals:
    void deleteRequested();

public slots:
    void setMalt(int i);

private:
    double _mass {0.};
    QComboBox* _nameBox;
    QDoubleSpinBox* _contribBox;
    QLabel* _massLabel;
    QPushButton* _deleteButton;
};

#endif // MALTWIDGET_H
