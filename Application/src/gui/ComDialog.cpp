#include "ComDialog.h"
#include "ui_ComDialog.h"

ComDialog::ComDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComDialog),
    _porta("")
{
    ui->setupUi(this);
}

ComDialog::~ComDialog()
{
    delete ui;
}

QString ComDialog::getPort()
{
    _porta = ui->lineEdit->text();
    return _porta;
}
