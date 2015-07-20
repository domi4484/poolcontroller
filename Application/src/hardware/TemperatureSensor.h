#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include <QVector>
#include <QPointF>

#include "AbstractTemperaturesensor.h"
#include "Arduino.h"

// Qt includes -----------------------------
#include <QStringList>

class QProcess;

class TemperatureSensor : public AbstractTemperatureSensor
{
Q_OBJECT
public:

   static const int TEMPERATURE_MINIMUM;
   static const int TEMPERATURE_MAXIMUM;

   TemperatureSensor(double delay_s);
   virtual ~TemperatureSensor();

   bool open(const QString &port);

    void addCalibrationPoint(double measured_temp);

    virtual QString sensorName() {return "Arduino";}

private:
    static const QString SCRIPT_CARATTERISTICA;

    Arduino *m_Arduino;
    QList<double> m_PolinomioCalibrazione;
    QProcess *m_QProcess_CalculatePolinome;
    QProcess *m_QProcess_CreateVoltageGraphic;
    bool m_ready;

    QStringList m_QList_Voltages;

    virtual double measure();
    double voltageToTemperature(double voltage);
    void retrievePolinomio();

private slots:

    void slot_retrievePolinomioFinished(int status);
    void slot_QProcess_CreateVoltageGraphic_finished(int status);
};

#endif // TEMPERATURESENSOR_H
