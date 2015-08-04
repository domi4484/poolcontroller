
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

const int TemperatureSensor::CONST_TEMPERATURE_MINIMUM (-30);
const int TemperatureSensor::CONST_TEMPERATURE_MAXIMUM (100);
const int TemperatureSensor::CONST_READ_DELAY_MS (500);

const QString TemperatureSensor::CONST_PATH_CALIBRATION_POINTS("calibrationData/CalibrationData.dat");
const QString TemperatureSensor::CONST_PATH_SCRIPT_CARATTERISTICA("./scripts/caratteristica.py");

const int TemperatureSensor::CONST_VOLTAGE_LIST_SIZE(30);

//-----------------------------------------------------------------------------------------------------------------------------

TemperatureSensor::TemperatureSensor() :
    QObject(),
    m_Temperature(0.0),
    m_Arduino(new Arduino()),
    m_QList_CalibrationPoints(),
    m_PolinomioCalibrazione(),
    m_QProcess_CalculatePolinome(new QProcess),
    m_QProcess_CreateVoltageGraphic(new QProcess),
    m_Status(Enum_Status_Offline),
    m_QTimer_ReadVoltage(this),
    m_QList_LiveVoltages()
{
  // Connections
  connect(m_QProcess_CalculatePolinome,
          SIGNAL(finished(int)),
          this,
          SLOT(slot_QProcess_retrievePolinomioFinished(int)));
  connect(m_QProcess_CreateVoltageGraphic,
          SIGNAL(finished(int)),
          this,
          SLOT(slot_QProcess_CreateVoltageGraphic_finished(int)));

  connect(&m_QTimer_ReadVoltage,
          SIGNAL(timeout()),
          SLOT(slot_QTimer_ReadVoltage_timeout()));

  // Load calibration points
  loadCalibrationPoints();
}

//-----------------------------------------------------------------------------------------------------------------------------

TemperatureSensor::~TemperatureSensor()
{
  delete m_QProcess_CreateVoltageGraphic;
  delete m_QProcess_CalculatePolinome;
  delete m_Arduino;
}

//-----------------------------------------------------------------------------------------------------------------------------

double TemperatureSensor::temperature() const
{
  return m_Temperature;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool TemperatureSensor::open(const QString &port)
{
  if(m_Arduino->open(port) == false)
  {
    m_Status = Enum_Status_Offline;
    m_QTimer_ReadVoltage.stop();
    return false;
  }

  setStatus(Enum_Status_Init);
  retrievePolinomio();
}

//-----------------------------------------------------------------------------------------------------------------------------

void TemperatureSensor::addCalibrationPoint(QPointF newPoint)
{
  m_QList_CalibrationPoints.append(newPoint);
  saveCalibrationPoints();

  setStatus(Enum_Status_Init);
}

//-----------------------------------------------------------------------------------------------------------------------------

QList<QPointF> TemperatureSensor::getCalibrationPoints()
{
  return m_QList_CalibrationPoints;
}

//-----------------------------------------------------------------------------------------------------------------------------

void TemperatureSensor::setStatus(TemperatureSensor::Enum_Status status)
{
  if(status == m_Status)
  {
    return;
  }

  m_Status = status;

  switch (m_Status)
  {
    case Enum_Status_Offline:
    {
      m_QTimer_ReadVoltage.stop();
      m_QList_LiveVoltages.clear();
    }
    break;

    case Enum_Status_Init:
      retrievePolinomio();
    break;

    case Enum_Status_Running:
    break;

    case Enum_Status_Stop:
    {
      m_QTimer_ReadVoltage.stop();
      m_QList_LiveVoltages.clear();
    }
    break;
  }

  emit signal_StatusChanged();
}

//-----------------------------------------------------------------------------------------------------------------------------

TemperatureSensor::Enum_Status TemperatureSensor::status() const
{
  return m_Status;
}

//-----------------------------------------------------------------------------------------------------------------------------

double TemperatureSensor::voltageToTemperature(double voltage)
{
  return m_PolinomioCalibrazione[2] + m_PolinomioCalibrazione[1]*voltage + m_PolinomioCalibrazione[0]*voltage*voltage;
}

//-----------------------------------------------------------------------------------------------------------------------------

void TemperatureSensor::loadCalibrationPoints()
{
  m_QList_CalibrationPoints.clear();

  QFile file(CONST_PATH_CALIBRATION_POINTS);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    Logger::error(tr("Can't find '%1'").arg(CONST_PATH_CALIBRATION_POINTS));
      return;
  }

  QTextStream in(&file);
  while (!in.atEnd())
  {
      QStringList point = in.readLine().split(";");
      m_QList_CalibrationPoints.append(QPointF(point.first().toDouble(),
                                               point.last().toDouble()));
  }

  file.close();
}

//-----------------------------------------------------------------------------------------------------------------------------

void TemperatureSensor::saveCalibrationPoints()
{
  QFile file(CONST_PATH_CALIBRATION_POINTS);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    Logger::error(tr("Can't find '%1'").arg(CONST_PATH_CALIBRATION_POINTS));
      return;
  }

  QStringList fileContent;
  foreach (QPointF point, m_QList_CalibrationPoints)
  {
    fileContent.append(QString("%1;%2").arg(point.x())
                                       .arg(point.y()));
  }

  file.write(fileContent.join("\n").toLatin1());

  file.close();
}

//-----------------------------------------------------------------------------------------------------------------------------

void TemperatureSensor::retrievePolinomio()
{
    Logger::info("Retrieving polinomio...");

    if(QFile::exists(CONST_PATH_SCRIPT_CARATTERISTICA) == false)
      Logger::error(tr("Script '%1' not found"));

    m_QProcess_CalculatePolinome->start(CONST_PATH_SCRIPT_CARATTERISTICA,
                                        QStringList() << CONST_PATH_CALIBRATION_POINTS);
}

//-----------------------------------------------------------------------------------------------------------------------------

void TemperatureSensor::slot_QProcess_retrievePolinomioFinished(int status)
{
    QString ret(m_QProcess_CalculatePolinome->readAllStandardOutput());
    QStringList rets = ret.split(" ");
    foreach(QString string, rets)
        m_PolinomioCalibrazione.append(string.toDouble());

    m_QTimer_ReadVoltage.start(CONST_READ_DELAY_MS);
}

//-----------------------------------------------------------------------------------------------------------------------------

void TemperatureSensor::slot_QProcess_CreateVoltageGraphic_finished(int status)
{

}

//-----------------------------------------------------------------------------------------------------------------------------

void TemperatureSensor::slot_QTimer_ReadVoltage_timeout()
{
  if(m_Arduino->readVoltage() == false)
  {
    Logger::error(tr("Can't read voltage on Arduino"));
    setStatus(Enum_Status_Offline);
    return;
  }

  // Read and filter voltage
  double voltage     = m_Arduino->get_A1_Voltage();
  double filtered = voltage;
  if(m_QList_LiveVoltages.isEmpty() == false)
  {
    double e = 0.3;
    filtered = (1-e) * m_QList_LiveVoltages.first().toDouble() + e * voltage;
  }

  m_Temperature = voltageToTemperature(filtered);

  m_QList_LiveVoltages.insert(0, QString::number(filtered));
  while(m_QList_LiveVoltages.size() > CONST_VOLTAGE_LIST_SIZE)
  {
    m_QList_LiveVoltages.removeLast();
  }

  // Still initializing?
  if(m_QList_LiveVoltages.size() < CONST_VOLTAGE_LIST_SIZE)
  {
    setStatus(Enum_Status_Init);
    return;
  }
  else
  {
    setStatus(Enum_Status_Running);
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

  if(   m_Temperature < CONST_TEMPERATURE_MINIMUM
     || m_Temperature > CONST_TEMPERATURE_MAXIMUM)
  {
    Logger::error(tr("Temperature out of possible range (%1)").arg(m_Temperature));
    return;
  }

  Logger::info(tr("Voltage: %1V Temperature: %2").arg(voltage)
                                                .arg(m_Temperature));
}





