
#include <QTimer>

#include "AbstractTemperaturesensor.h"

#include "../HelperClasses/Logger.h"

//-----------------------------------------------------------------------------------------------------------------------------

AbstractTemperatureSensor::AbstractTemperatureSensor(double delay_s) :
    QObject(),
    m_temperature(0),
    m_counter    (0)
{
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_measure()));
    timer->start(delay_s*1000);  // in ms
}

//-----------------------------------------------------------------------------------------------------------------------------

QVector<QPointF> AbstractTemperatureSensor::getData()
{
    return m_vettore;
}

//-----------------------------------------------------------------------------------------------------------------------------

double AbstractTemperatureSensor::getLast()
{
    return m_temperature;
}

//-----------------------------------------------------------------------------------------------------------------------------

void AbstractTemperatureSensor::slot_measure()
{
    m_temperature = measure();

    m_vettore.append(QPointF(m_counter++, m_temperature));
}

//-----------------------------------------------------------------------------------------------------------------------------







