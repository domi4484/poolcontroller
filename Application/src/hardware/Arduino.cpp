
#include "Arduino.h"

#include "../HelperClasses/Logger.h"

#include <QDebug>

#include "qextserialport.h"

//-----------------------------------------------------------------------------------------------------------------------------

Arduino::Arduino() :
  m_A1_Voltage(-1.0)
{
}

//-----------------------------------------------------------------------------------------------------------------------------

Arduino::~Arduino()
{
    m_QExtSerialPort->close();
}

//-----------------------------------------------------------------------------------------------------------------------------

bool Arduino::open(const QString &port_name)
{
  Logger::info(QString("Initializing Arduino device on port: %1").arg(port_name));
  m_QExtSerialPort = new QextSerialPort(port_name);
  m_QExtSerialPort->setBaudRate(BAUD9600);
  m_QExtSerialPort->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

  return m_QExtSerialPort->isOpen();
}

//-----------------------------------------------------------------------------------------------------------------------------

bool Arduino::readVoltage()
{
    if(send("1\n") == false)
    {
      return false;
    }

    QString answer = receive();
    m_A1_Voltage = 5.0*answer.toLongLong()/1024.0;

    return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

double Arduino::get_A1_Voltage()
{
  return m_A1_Voltage;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool Arduino::send(const QString &message)
{
    int i = m_QExtSerialPort->write(message.toAscii(),
                          message.length());

    if(i == -1)
    {
      Logger::error("Arduino::send write error");
      return false;
    }

    return true;
}

//-----------------------------------------------------------------------------------------------------------------------------

QString Arduino::receive()
{
    char buff[1024];
    int numBytes;

    numBytes = m_QExtSerialPort->bytesAvailable();
    if(numBytes > 1024)
        numBytes = 1024;

    int i = m_QExtSerialPort->read(buff, numBytes);
    if (i != -1)
        buff[i] = '\0';
    else
        buff[0] = '\0';

    return QLatin1String(buff);
}

//-----------------------------------------------------------------------------------------------------------------------------
