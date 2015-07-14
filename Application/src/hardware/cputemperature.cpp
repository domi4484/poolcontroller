#include <QProcess>
#include <QTimer>
#include <QFile>

#include "../HelperClasses/Logger.h"

#include "CpuTemperature.h"

const QString CpuTemperature::READER_SCRIPT("scripts/cpuTemp");

//-----------------------------------------------------------------------------------------------------------------------------

CpuTemperature::CpuTemperature(double delay) :
    AbstractTemperatureSensor(delay),
    m_processo(new QProcess())
{
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_readCpuTemp()));
    timer->start(1000);  // in ms

    connect(m_processo, SIGNAL(finished(int)), this, SLOT(slot_processFinished(int)));
}

//-----------------------------------------------------------------------------------------------------------------------------

double CpuTemperature::measure()
{
    return m_temperature;
}

//-----------------------------------------------------------------------------------------------------------------------------

void CpuTemperature::slot_readCpuTemp()
{
    if (QFile::exists(READER_SCRIPT))
        m_processo->start(READER_SCRIPT);
    else
        Logger::error(QString("Script %1 non trovato").arg(READER_SCRIPT));
}

//-----------------------------------------------------------------------------------------------------------------------------

void CpuTemperature::slot_processFinished(int)
{
    QString ret(m_processo->readAllStandardOutput());
    QStringList rets = ret.split("+");
    m_temperature = rets[1].split("°")[0].toDouble();
}

//-----------------------------------------------------------------------------------------------------------------------------
