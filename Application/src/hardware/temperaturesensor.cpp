
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QProcess>

#include "../HelperClasses/Logger.h"

#include "TemperatureSensor.h"
#include "Arduino.h"

//-----------------------------------------------------------------------------------------------------------------------------

TemperatureSensor::TemperatureSensor(int delay) :
    AbstractTemperatureSensor(delay),
    m_arduino(new Arduino()),
    m_polinomio(),
    m_process(new QProcess),
    m_ready(false)
{
    connect(m_process, SIGNAL(finished(int)), this, SLOT(slot_retrievePolinomioFinished(int)));

    retrievePolinomio();
}

//-----------------------------------------------------------------------------------------------------------------------------

TemperatureSensor::~TemperatureSensor()
{
  delete m_arduino;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool TemperatureSensor::open(const QString &port)
{
  return m_arduino->open(port);
}

//-----------------------------------------------------------------------------------------------------------------------------

//void TemperatureSensor::addCalibrationPoint(double measured_temp)
//{
//}

//-----------------------------------------------------------------------------------------------------------------------------

double TemperatureSensor::voltageToTemperature(double voltage)
{
    return m_polinomio[0] + m_polinomio[1]*voltage + m_polinomio[2]*voltage*voltage;
}

//-----------------------------------------------------------------------------------------------------------------------------

double TemperatureSensor::measure()
{
    if (!m_ready)
        return 0;
    return voltageToTemperature(m_arduino->readVoltage());
}

//-----------------------------------------------------------------------------------------------------------------------------

void TemperatureSensor::retrievePolinomio()
{
    Logger::info("Retrieving polinomio...");

    QString script = "./scripts/caratteristica.py";

    if(QFile::exists(script) == false)
      Logger::error(tr("Script '%1' not found"));

    m_process->start("./scripts/caratteristica.py");
}

//-----------------------------------------------------------------------------------------------------------------------------

void TemperatureSensor::slot_retrievePolinomioFinished(int status)
{
    Logger::info(QString("Process status: %1").arg(status));
    QString ret(m_process->readAllStandardOutput());
    Logger::info(QString("Polinomio: %1").arg(ret));
    QStringList rets = ret.split(" ");
    foreach(QString string, rets)
        m_polinomio.append(string.toDouble());
    m_ready = true;
}
