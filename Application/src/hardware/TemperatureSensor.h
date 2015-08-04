#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

// Project includes ------------------------
#include "Arduino.h"

// Qt includes -----------------------------
#include <QMap>
#include <QPointF>
#include <QStringList>
#include <QTimer>

class QProcess;

class TemperatureSensor : public QObject
{
Q_OBJECT
public:

   static const int CONST_TEMPERATURE_MINIMUM;
   static const int CONST_TEMPERATURE_MAXIMUM;
   static const int CONST_READ_DELAY_MS;

   enum Enum_Status
   {
     Enum_Status_Offline,
     Enum_Status_Init,
     Enum_Status_Running,
     Enum_Status_Stop
   };

   TemperatureSensor();
   virtual ~TemperatureSensor();

   double temperature() const;

   bool open(const QString &port);

   void addCalibrationPoint(QPointF newPoint);
   QList<QPointF> getCalibrationPoints();

   virtual QString sensorName() {return "Arduino";}

   void setStatus(Enum_Status status);
   Enum_Status status() const;

signals:

   void signal_StatusChanged();

private slots:

    void slot_QProcess_retrievePolinomioFinished(int status);
    void slot_QProcess_CreateVoltageGraphic_finished(int status);

    void slot_QTimer_ReadVoltage_timeout();

private:

    static const QString CONST_PATH_CALIBRATION_POINTS;
    static const QString CONST_PATH_SCRIPT_CARATTERISTICA;

    static const int CONST_VOLTAGE_LIST_SIZE;

    double m_Temperature;
    Arduino *m_Arduino;
    QList<QPointF> m_QList_CalibrationPoints;
    QList<double> m_PolinomioCalibrazione;
    QProcess *m_QProcess_CalculatePolinome;
    QProcess *m_QProcess_CreateVoltageGraphic;
    Enum_Status m_Status;

    QTimer m_QTimer_ReadVoltage;
    QStringList m_QList_LiveVoltages;


    double voltageToTemperature(double voltage);
    void loadCalibrationPoints();
    void saveCalibrationPoints();
    void retrievePolinomio();
};

#endif // TEMPERATURESENSOR_H
