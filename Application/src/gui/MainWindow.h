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

public slots:
    void on_actionAggiungi_punto_di_calibrazione_triggered();

    void updateGraph();

    void setupCalibrationTab();

signals:
    void newTemp(double newValue);

private:

    static const QString PORT_1;
    static const QString PORT_2;
    static const double  DELAY_S;

    Ui::MainWindow *ui;
    std::ofstream* m_log;
    TemperatureSensor* m_temperatureSensor;
};

#endif // MAINWINDOW_H
