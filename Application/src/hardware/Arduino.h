#ifndef ARDUINO_H
#define ARDUINO_H

#include <QString>

class QSerialPort;

class Arduino
{
public:
    Arduino();
    ~Arduino();

    bool open(const QString &port_name);

    bool readVoltage();

    double get_A1_Voltage();

private:

    QSerialPort *m_QExtSerialPort;

    double m_A1_Voltage;

    bool send(const QString &message);
    QString receive();
};

#endif // ARDUINO_H
