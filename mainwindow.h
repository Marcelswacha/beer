#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QVector>

class QDoubleSpinBox;
class QLabel;
class QPushButton;
class QTextEdit;
class QVBoxLayout;

class HopWidget;
class MaltWidget;
class Recipy;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void compute();

    void onAddMalt();
    void onAddHop();
    void onDeleteMalt();
    void onDeleteHop();

    void addRecipy();
    void clearRecipies();

private:
    // malt
    QDoubleSpinBox* _volume;
    QDoubleSpinBox* _volumeLoss;
    QDoubleSpinBox* _blg;
    QDoubleSpinBox* _efficiency;
    QLabel*         _ebu;
    QVector<MaltWidget*> _maltWidgets;
    QPushButton* _addMaltButton;
    QPushButton* _computeButton;
    QVBoxLayout* _maltLayout;

    // hop
    QVBoxLayout* _hopLayout;
    QPushButton* _addHopButton;
    QVector<HopWidget*> _hopWidgets;

    // recipies
    QVector<Recipy> _recipies;
    QTextEdit* _textEdit;

    void printRecipies();

};

#endif // MAINWINDOW_H
