#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include <QVector>
#include <QPointF>

#include "AbstractTemperaturesensor.h"
#include "Arduino.h"

class QProcess;

class TemperatureSensor : public AbstractTemperatureSensor
{
Q_OBJECT
public:

   TemperatureSensor(int delay);
   virtual ~TemperatureSensor();

   bool open(const QString &port);

    void addCalibrationPoint(double measured_temp);

    virtual QString sensorName() {return "Arduino";}

private:
    static const QString SCRIPT_CARATTERISTICA;

    Arduino *m_arduino;
    QList<double> m_polinomio;
    QProcess *m_process;
    bool m_ready;

    virtual double measure();
    double voltageToTemperature(double voltage);
    void retrievePolinomio();

private slots:
    void slot_retrievePolinomioFinished(int status);
};

#endif // TEMPERATURESENSOR_H
