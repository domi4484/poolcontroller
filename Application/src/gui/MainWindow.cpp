#include <QFileDialog>
#include <QFile>
#include <QDateTime>
#include <QColorGroup>
#include <QPalette>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "../HelperClasses/Logger.h"

#include "../hardware/TemperatureSensor.h"
#include "../hardware/CpuTemperature.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------------------------------

const int INTERVALLO_UPDATE_GRAFICO = 5; // Ogni quanti secondi faccio un refresh del grafico
const int INTERVALLO_LOG = 60; // Updates del log file in secondi
const int MAX = 70;
const int MIN = -10;

//-----------------------------------------------------------------------------------------------------------------------------

const QString MainWindow::PORT_1 ("/dev/ttyACM0");
const QString MainWindow::PORT_2 ("/dev/");
const int     MainWindow::DELAY_S(1);

//-----------------------------------------------------------------------------------------------------------------------------

MainWindow::MainWindow(bool emulate_hardware,
                       QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_dialogAddCalibrationPoint(NULL),
    m_log(NULL),
    m_temperatureSensor(NULL)
{
   Logger::instanziate(Logger::LOG_VERBOSE);
   Logger::info("Started application");

   ui->setupUi(this);


    QPalette palette = ui->lcdNumber->palette();
    palette.setColor(QPalette::Normal, QPalette::Foreground, Qt::red);
    palette.setColor(QPalette::Normal, QPalette::Light, Qt::black);
    palette.setColor(QPalette::Normal, QPalette::Dark, Qt::darkYellow);
    ui->lcdNumber->setPalette(palette);


    //-----------------------------------------------------------------------------------------------------------------------------
    // Initialize hardware
    //-----------------------------------------------------------------------------------------------------------------------------
    if (emulate_hardware == false)
    {
      TemperatureSensor *temperatureSensor = new TemperatureSensor(DELAY_S);

      Logger::info(QString("Try to open Temperature sensor on port '%1'").arg(PORT_1));
      if(temperatureSensor->open(PORT_1) == false)
      {
         Logger::info(QString("No sensor found on port '%1'").arg(PORT_1));

         Logger::info(QString("Try to open Temperature sensor on port '%1'").arg(PORT_2));
         if(temperatureSensor->open(PORT_2) == false)
         {
           Logger::info("Can't open any sensor");
         }
      }

      m_temperatureSensor = temperatureSensor;
    }
    else
    {
       Logger::info("Start reading temperature from cpu");
       m_temperatureSensor = new CpuTemperature(DELAY_S);
       ui->actionAggiungi_punto_di_calibrazione->setDisabled(true);
    }

    m_dialogAddCalibrationPoint = new DialogAddCalibrationPoint(this, m_temperatureSensor);

    QDateTime tempo = QDateTime::currentDateTime();
    ui->lbl_startTime->setText(tempo.time().toString());


    // Grafico
    QTimer* timerGraph = new QTimer(this);
    connect(timerGraph, SIGNAL(timeout()), this, SLOT(updateGraph()));
    timerGraph->start(INTERVALLO_UPDATE_GRAFICO * 1000);  // update del grafico ogni 10 secondi
}

//-----------------------------------------------------------------------------------------------------------------------------

MainWindow::~MainWindow()
{
    delete ui;
}

//-----------------------------------------------------------------------------------------------------------------------------

/*  Browse for a logfile */
void MainWindow::on_btn_browse_clicked()
{
    QFileDialog fileDialog(this);
    fileDialog.setDirectory("/home/domi/Progetti/cascinaKeko");
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setNameFilter(tr("*.dat"));
    QStringList fileNames;
    if (fileDialog.exec())
    {
        fileNames =fileDialog.selectedFiles();
        QString fileName = fileNames[0];
        ui->edit_log->setText(fileName);
        m_log = new ofstream(fileName.toStdString().c_str(), ios::app);
    }
    else
        m_log = NULL;

}

//-----------------------------------------------------------------------------------------------------------------------------

/* Open the dialog to add a new calibration point */
void MainWindow::on_actionAggiungi_punto_di_calibrazione_triggered()
{
    m_dialogAddCalibrationPoint->show();
}

//-----------------------------------------------------------------------------------------------------------------------------

/* Update the Chart */
void MainWindow::updateGraph()
{
    ui->lcdNumber->display(m_temperatureSensor->getLast());
}

//-----------------------------------------------------------------------------------------------------------------------------

