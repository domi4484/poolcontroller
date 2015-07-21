
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QProcess>

#include "../HelperClasses/Logger.h"

#include "TemperatureSensor.h"
#include "Arduino.h"

#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------------------

const int TemperatureSensor::TEMPERATURE_MINIMUM (-30);
const int TemperatureSensor::TEMPERATURE_MAXIMUM (100);

const QString TemperatureSensor::PATH_CALIBRATION_POINTS("calibrationData/CalibrationData.dat");
const QString TemperatureSensor::PATH_SCRIPT_CARATTERISTICA("./scripts/caratteristica.py");

//-----------------------------------------------------------------------------------------------------------------------------

TemperatureSensor::TemperatureSensor(double delay_s) :
    AbstractTemperatureSensor(delay_s),
    m_Arduino(new Arduino()),
    m_QMap_CalibrationPoints(),
    m_PolinomioCalibrazione(),
    m_QProcess_CalculatePolinome(new QProcess),
    m_QProcess_CreateVoltageGraphic(new QProcess),
    m_ready(false),
    m_QList_LiveVoltages()
{
    connect(m_QProcess_CalculatePolinome,
            SIGNAL(finished(int)),
            this,
            SLOT(slot_retrievePolinomioFinished(int)));
    connect(m_QProcess_CreateVoltageGraphic,
            SIGNAL(finished(int)),
            this,
            SLOT(slot_QProcess_CreateVoltageGraphic_finished(int)));


    loadCalibrationPoints();
    retrievePolinomio();
}

//-----------------------------------------------------------------------------------------------------------------------------

TemperatureSensor::~TemperatureSensor()
{
  delete m_Arduino;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool TemperatureSensor::open(const QString &port)
{
  return m_Arduino->open(port);
}

//-----------------------------------------------------------------------------------------------------------------------------

//void TemperatureSensor::addCalibrationPoint(double measured_temp)
//{
//}

//-----------------------------------------------------------------------------------------------------------------------------

double TemperatureSensor::voltageToTemperature(double voltage)
{
  return m_PolinomioCalibrazione[2] + m_PolinomioCalibrazione[1]*voltage + m_PolinomioCalibrazione[0]*voltage*voltage;
}

//-----------------------------------------------------------------------------------------------------------------------------

void TemperatureSensor::loadCalibrationPoints()
{
  m_QMap_CalibrationPoints.clear();

  QFile file(PATH_CALIBRATION_POINTS);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    Logger::error(tr("Can't find '%1'").arg(PATH_CALIBRATION_POINTS));
      return;
  }

  QTextStream in(&file);
  while (!in.atEnd())
  {
      QStringList point = in.readLine().split(";");
      m_QMap_CalibrationPoints.insert(point.first().toDouble(), point.last().toDouble());
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

double TemperatureSensor::measure()
{
    if (!m_ready)
        return 0;

    if(m_Arduino->readVoltage() == false)
    {
      Logger::error(tr("Can't read voltage on Arduino"));
      return false;
    }

    double voltage     = m_Arduino->get_A1_Voltage();
    double filtered = voltage;
    if(m_QList_LiveVoltages.isEmpty() == false)
    {
      double e = 0.3;
      filtered = (1-e) * m_QList_LiveVoltages.first().toDouble() + e * voltage;
    }

    double temperature = voltageToTemperature(filtered);

    m_QList_LiveVoltages.insert(0, QString::number(filtered));
    while(m_QList_LiveVoltages.size() > 30)
    {
      m_QList_LiveVoltages.removeLast();
    }

    QFile qFile("VoltageData.dat");
    if (!qFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      Logger::warning(tr("Can't save voltage data on '%1'").arg(qFile.fileName()));
    }
    else
    {
      qFile.write(m_QList_LiveVoltages.join("\n").toLatin1());

      m_QProcess_CreateVoltageGraphic->start("./scripts/onlineVoltage.py");
    }

    if(   temperature < TEMPERATURE_MINIMUM
       || temperature > TEMPERATURE_MAXIMUM)
    {
      Logger::error(tr("Temperature out of possible range (%1)").arg(temperature));
      return false;
    }

    Logger::info(tr("Voltage: %1V Temperature: %2").arg(voltage)
                                                  .arg(temperature));

    return temperature;
}

//-----------------------------------------------------------------------------------------------------------------------------

void TemperatureSensor::retrievePolinomio()
{
    Logger::info("Retrieving polinomio...");

    if(QFile::exists(PATH_SCRIPT_CARATTERISTICA) == false)
      Logger::error(tr("Script '%1' not found"));

    m_QProcess_CalculatePolinome->start(PATH_SCRIPT_CARATTERISTICA,
                                        QStringList() << PATH_CALIBRATION_POINTS);
}

//-----------------------------------------------------------------------------------------------------------------------------

void TemperatureSensor::slot_retrievePolinomioFinished(int status)
{
    Logger::info(QString("Process status: %1").arg(status));
    QString ret(m_QProcess_CalculatePolinome->readAllStandardOutput());
    Logger::info(QString("Polinomio: %1").arg(ret));
    QStringList rets = ret.split(" ");
    foreach(QString string, rets)
        m_PolinomioCalibrazione.append(string.toDouble());
    m_ready = true;
}

//-----------------------------------------------------------------------------------------------------------------------------

void TemperatureSensor::slot_QProcess_CreateVoltageGraphic_finished(int status)
{

}
