#ifndef ABSTRACTTEMPERATURESENSOR_H
#define ABSTRACTTEMPERATURESENSOR_H

#include <QObject>
#include <QVector>
#include <QPointF>

class AbstractTemperatureSensor : public QObject
{
    Q_OBJECT
public:
    AbstractTemperatureSensor(double delay_s);
    QVector<QPointF> getData();
    double getLast();
//    virtual void addCalibrationPoint(double measured_temp){;}

protected:
    double m_temperature;

private:

    int              m_counter;
    QVector<QPointF> m_vettore;

    virtual double measure() = 0;

private slots:
    void slot_measure();
};

#endif // ABSTRACTTEMPERATURESENSOR_H
