#ifndef COMDIALOG_H
#define COMDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
    class ComDialog;
}

class ComDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ComDialog(QWidget *parent = 0);
    ~ComDialog();

    QString getPort();

private:
    Ui::ComDialog *ui;

    QString _porta;
};

#endif // COMDIALOG_H
