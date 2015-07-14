#ifndef ARDUINO_H
#define ARDUINO_H

#include <QString>

class QextSerialPort;

class Arduino
{
public:
    Arduino();
    ~Arduino();

    bool open(const QString &port_name);

    double readVoltage();

private:
    QextSerialPort *m_port;

    void send(const QString &message);
    QString receive();
};

#endif // ARDUINO_H
