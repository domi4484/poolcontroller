
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QProcess>

#include "../HelperClasses/Logger.h"

#include "TemperatureSensor.h"
#include "Arduino.h"

//-----------------------------------------------------------------------------------------------------------------------------

const int TemperatureSensor::TEMPERATURE_MINIMUM (-30);
const int TemperatureSensor::TEMPERATURE_MAXIMUM (100);

//-----------------------------------------------------------------------------------------------------------------------------

TemperatureSensor::TemperatureSensor(int delay) :
    AbstractTemperatureSensor(delay),
    m_Arduino(new Arduino()),
    m_PolinomioCalibrazione(),
    m_QProcess(new QProcess),
    m_ready(false),
    m_QList_Voltages()
{
    connect(m_QProcess,
            SIGNAL(finished(int)),
            this,
            SLOT(slot_retrievePolinomioFinished(int)));

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
    return m_PolinomioCalibrazione[0] + m_PolinomioCalibrazione[1]*voltage + m_PolinomioCalibrazione[2]*voltage*voltage;
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
    double temperature = voltageToTemperature(voltage);


    m_QList_Voltages.insert(0, QString::number(voltage));
    while(m_QList_Voltages.size() > 20)
    {
      m_QList_Voltages.removeLast();
    }

    QFile qFile("VoltageData.dat");
    if (!qFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      Logger::warning(tr("Can't save voltage data on '%1'").arg(qFile.fileName()));
    }
    else
    {
      qFile.write(m_QList_Voltages.join("\n").toLatin1());
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

    QString script = "./scripts/caratteristica.py";

    if(QFile::exists(script) == false)
      Logger::error(tr("Script '%1' not found"));

    m_QProcess->start("./scripts/caratteristica.py");
}

//-----------------------------------------------------------------------------------------------------------------------------

void TemperatureSensor::slot_retrievePolinomioFinished(int status)
{
    Logger::info(QString("Process status: %1").arg(status));
    QString ret(m_QProcess->readAllStandardOutput());
    Logger::info(QString("Polinomio: %1").arg(ret));
    QStringList rets = ret.split(" ");
    foreach(QString string, rets)
        m_PolinomioCalibrazione.append(string.toDouble());
    m_ready = true;
}
