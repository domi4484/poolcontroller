#ifndef CPUTEMPERATURE_H
#define CPUTEMPERATURE_H

#include <QVector>
#include <QPointF>

#include "AbstractTemperaturesensor.h"

class QProcess;

class CpuTemperature : public AbstractTemperatureSensor
{
    Q_OBJECT
public:
    CpuTemperature(double delay);
    virtual QString sensorName() {return "Cpu";}

private:
    QProcess *m_processo;

    virtual double measure();

    static const QString READER_SCRIPT;

private slots:
    void slot_readCpuTemp();
    void slot_processFinished(int);
};

#endif // CPUTEMPERATURE_H
