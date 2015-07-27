#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include <QVector>
#include <QPointF>

#include "AbstractTemperaturesensor.h"
#include "Arduino.h"

// Qt includes -----------------------------
#include <QStringList>
#include <QMap>

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

   void addCalibrationPoint(QPointF newPoint);
   QList<QPointF> getCalibrationPoints();

   virtual QString sensorName() {return "Arduino";}

private slots:

    void slot_retrievePolinomioFinished(int status);
    void slot_QProcess_CreateVoltageGraphic_finished(int status);

private:

    static const QString PATH_CALIBRATION_POINTS;
    static const QString PATH_SCRIPT_CARATTERISTICA;

    Arduino *m_Arduino;
    QList<QPointF> m_QList_CalibrationPoints;
    QList<double> m_PolinomioCalibrazione;
    QProcess *m_QProcess_CalculatePolinome;
    QProcess *m_QProcess_CreateVoltageGraphic;
    bool m_ready;

    QStringList m_QList_LiveVoltages;


    virtual double measure();
    double voltageToTemperature(double voltage);
    void loadCalibrationPoints();
    void saveCalibrationPoints();
    void retrievePolinomio();
};

#endif // TEMPERATURESENSOR_H
