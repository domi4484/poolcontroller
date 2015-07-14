#ifndef DIALOGADDCALIBRATIONPOINT_H
#define DIALOGADDCALIBRATIONPOINT_H

#include <QDialog>

namespace Ui {
class DialogAddCalibrationPoint;
}

class AbstractTemperatureSensor;

class DialogAddCalibrationPoint : public QDialog
{
    Q_OBJECT
    
public:
    DialogAddCalibrationPoint(QWidget *parent, AbstractTemperatureSensor *sensor);
    ~DialogAddCalibrationPoint();
    
private:
    Ui::DialogAddCalibrationPoint *ui;
    AbstractTemperatureSensor *m_sensor;

public slots:
    void slot_voltage(double voltage);
private slots:
    void on_buttonBox_accepted();
};

#endif // DIALOGADDCALIBRATIONPOINT_H
