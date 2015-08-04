
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "../HelperClasses/Logger.h"

#include "../hardware/TemperatureSensor.h"

#include <QFileDialog>
#include <QFile>
#include <QDateTime>
#include <QMessageBox>

//-----------------------------------------------------------------------------------------------------------------------------

const int INTERVALLO_UPDATE_GRAFICO = 1; // Ogni quanti secondi faccio un refresh del grafico
const int INTERVALLO_LOG = 60; // Updates del log file in secondi
const int MAX = 70;
const int MIN = -10;


//-----------------------------------------------------------------------------------------------------------------------------

const QString MainWindow::PORT_1 ("/dev/ttyACM0");
const QString MainWindow::PORT_2 ("/dev/");

//-----------------------------------------------------------------------------------------------------------------------------

MainWindow::MainWindow(bool emulate_hardware,
                       QWidget *parent) :
    QMainWindow(parent),
    m_Ui(new Ui::MainWindow),
    m_log(NULL),
    m_TemperatureSensor(NULL)
{
   Logger::instanziate(Logger::LOG_VERBOSE);
   Logger::info("Started application");

   m_Ui->setupUi(this);


    QPalette palette = m_Ui->lcdNumber->palette();
    palette.setColor(QPalette::Normal, QPalette::Foreground, Qt::red);
    palette.setColor(QPalette::Normal, QPalette::Light, Qt::black);
    palette.setColor(QPalette::Normal, QPalette::Dark, Qt::darkYellow);
    m_Ui->lcdNumber->setPalette(palette);


    //-----------------------------------------------------------------------------------------------------------------------------
    // Initialize hardware
    //-----------------------------------------------------------------------------------------------------------------------------
    m_TemperatureSensor = new TemperatureSensor();

    Logger::info(QString("Try to open Temperature sensor on port '%1'").arg(PORT_1));
    if(m_TemperatureSensor->open(PORT_1) == false)
    {
      Logger::info(QString("No sensor found on port '%1'").arg(PORT_1));

      Logger::info(QString("Try to open Temperature sensor on port '%1'").arg(PORT_2));
      if(m_TemperatureSensor->open(PORT_2) == false)
      {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Can't open temperature sensor"));
      }
    }

    // Connections
    connect(m_TemperatureSensor,
            SIGNAL(signal_StatusChanged()),
            SLOT(slot_TemperatureSensor_StatusChanged()));
    slot_TemperatureSensor_StatusChanged();


    // Grafico
    QTimer* timerGraph = new QTimer(this);
    connect(timerGraph, SIGNAL(timeout()), this, SLOT(updateGraph()));
    timerGraph->start(INTERVALLO_UPDATE_GRAFICO * 1000);  // update del grafico ogni 10 secondi

    setupCalibrationTab();
}

//-----------------------------------------------------------------------------------------------------------------------------

MainWindow::~MainWindow()
{
  delete m_Ui;
}

//-----------------------------------------------------------------------------------------------------------------------------

void MainWindow::slot_TemperatureSensor_StatusChanged()
{
  switch (m_TemperatureSensor->status())
  {
    case TemperatureSensor::Enum_Status_Offline:
    {
      m_Ui->m_QStatusBar->showMessage(tr("Offline"));
    }
    break;

    case TemperatureSensor::Enum_Status_Init:
    {
      m_Ui->m_QStatusBar->showMessage(tr("Init"));
    }
    break;

    case TemperatureSensor::Enum_Status_Running:
    {
      m_Ui->m_QStatusBar->showMessage(tr("Running"));
    }
    break;
    case TemperatureSensor::Enum_Status_Stop:
    {
      m_Ui->m_QStatusBar->showMessage(tr("Stop"));
    }
    break;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------

void MainWindow::updateGraph()
{
    m_Ui->lcdNumber->display(m_TemperatureSensor->temperature());
    m_Ui->m_QLabel_GraphicLive->setPixmap(QPixmap("voltage.png"));
}

//-----------------------------------------------------------------------------------------------------------------------------

void MainWindow::setupCalibrationTab()
{
  m_Ui->m_QTreeWidget_CalibrationPoints->clear();

  QList<QPointF> calibrationPoints = m_TemperatureSensor->getCalibrationPoints();

  foreach (QPointF point, calibrationPoints)
  {
    QTreeWidgetItem *qTreeWidgetItem = new QTreeWidgetItem(m_Ui->m_QTreeWidget_CalibrationPoints);
    qTreeWidgetItem->setText(0, QString::number(point.x()));
    qTreeWidgetItem->setText(1, QString::number(point.y()));
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

