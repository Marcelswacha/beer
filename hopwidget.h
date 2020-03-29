#ifndef HOPWIDGET_H
#define HOPWIDGET_H

#include <QWidget>
#include <QString>

class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QPushButton;

class HopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HopWidget(QWidget *parent = 0);

    QString getName() const;
    double getAlpha() const;
    double getMass() const;
    double getTime() const;

signals:
    void deleteRequested();

public slots:
    void setHop(int i);
    void onHopChanged(int);

private:
    QComboBox* _nameBox;
    QDoubleSpinBox* _alphaBox;
    QDoubleSpinBox* _massBox;
    QDoubleSpinBox* _timeBox;
    QPushButton* _deleteButton;
};

#endif // HOPWIDGET_H
