/********************************************************************
  Main Window

  Description:  Main Thread
  Author:       Damiano Lombardi
  Created:      06.02.2012

  Copiright (c) 2012 Damiano Lombardi
********************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui { class MainWindow; }
class TemperatureSensor;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(bool emulate, QWidget *parent = 0);
    ~MainWindow();
    double getVoltage();

signals:
    void newTemp(double newValue);

private slots:

    void slot_TemperatureSensor_StatusChanged();

    void updateGraph();
    void setupCalibrationTab();

private:

    static const QString PORT_1;
    static const QString PORT_2;

    Ui::MainWindow *m_Ui;

    TemperatureSensor* m_TemperatureSensor;

    QTimer m_QTimer_UpdateGraphics;
};

#endif // MAINWINDOW_H
