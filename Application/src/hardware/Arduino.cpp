
#include "Arduino.h"

#include "../HelperClasses/Logger.h"

#include <QDebug>

#include "qextserialport.h"

//-----------------------------------------------------------------------------------------------------------------------------

Arduino::Arduino()
{
}

//-----------------------------------------------------------------------------------------------------------------------------

Arduino::~Arduino()
{
    m_port->close();
    qDebug("is open: %d", m_port->isOpen());
}

//-----------------------------------------------------------------------------------------------------------------------------

bool Arduino::open(const QString &port_name)
{
  Logger::info(QString("Initializing Arduino device on port: %1").arg(port_name));
  m_port = new QextSerialPort(port_name);
  m_port->setBaudRate(BAUD9600);
  m_port->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

  return m_port->isOpen();
}

//-----------------------------------------------------------------------------------------------------------------------------

double Arduino::readVoltage()
{
    send("1\n");
    QString answer = receive();
    return 5.0*answer.toLongLong()/1024;
}

//-----------------------------------------------------------------------------------------------------------------------------

void Arduino::send(const QString &message)
{
    int i = m_port->write(message.toAscii(),
                          message.length());

    if(i == -1)
      Logger::error("Arduino::send write error");
}

//-----------------------------------------------------------------------------------------------------------------------------

QString Arduino::receive()
{
    char buff[1024];
    int numBytes;

    numBytes = m_port->bytesAvailable();
    if(numBytes > 1024)
        numBytes = 1024;

    int i = m_port->read(buff, numBytes);
    if (i != -1)
        buff[i] = '\0';
    else
        buff[0] = '\0';

    qDebug("bytes available: %d", numBytes);
    qDebug("received: %d", i);
    return QLatin1String(buff);
}

//-----------------------------------------------------------------------------------------------------------------------------
