
// Files includes --------------------------
#include "DialogAddCalibrationPoint.h"
#include "ui_DialogAddCalibrationPoint.h"

// Project includes ------------------------
#include "AbstractTemperaturesensor.h"

DialogAddCalibrationPoint::DialogAddCalibrationPoint(QWidget *parent, AbstractTemperatureSensor *sensor) :
    QDialog(parent),
    ui(new Ui::DialogAddCalibrationPoint),
    m_sensor(sensor)
{
    ui->setupUi(this);
}

DialogAddCalibrationPoint::~DialogAddCalibrationPoint()
{
    delete ui;
}

void DialogAddCalibrationPoint::slot_voltage(double voltage)
{
    ui->label_voltage->setNum(voltage);
}

void DialogAddCalibrationPoint::on_buttonBox_accepted()
{
//    m_sensor->addCalibrationPoint(ui->doubleSpinBox->value());
}
