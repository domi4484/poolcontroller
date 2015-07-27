
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
const double  MainWindow::DELAY_S(1.0);

//-----------------------------------------------------------------------------------------------------------------------------

MainWindow::MainWindow(bool emulate_hardware,
                       QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
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
    m_temperatureSensor = new TemperatureSensor(DELAY_S);

    Logger::info(QString("Try to open Temperature sensor on port '%1'").arg(PORT_1));
    if(m_temperatureSensor->open(PORT_1) == false)
    {
      Logger::info(QString("No sensor found on port '%1'").arg(PORT_1));

      Logger::info(QString("Try to open Temperature sensor on port '%1'").arg(PORT_2));
      if(m_temperatureSensor->open(PORT_2) == false)
      {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Can't find temperature sensor"));
        QApplication::exit(1);
        return;
      }
    }

    // Grafico
    QTimer* timerGraph = new QTimer(this);
    connect(timerGraph, SIGNAL(timeout()), this, SLOT(updateGraph()));
    timerGraph->start(INTERVALLO_UPDATE_GRAFICO * 1000);  // update del grafico ogni 10 secondi

    setupCalibrationTab();
}

//-----------------------------------------------------------------------------------------------------------------------------

MainWindow::~MainWindow()
{
  delete ui;
}

//-----------------------------------------------------------------------------------------------------------------------------

/* Open the dialog to add a new calibration point */
void MainWindow::on_actionAggiungi_punto_di_calibrazione_triggered()
{
}

//-----------------------------------------------------------------------------------------------------------------------------

/* Update the Chart */
void MainWindow::updateGraph()
{
    ui->lcdNumber->display(m_temperatureSensor->getLast());
    ui->m_QLabel_GraphicLive->setPixmap(QPixmap("voltage.png"));
}

//-----------------------------------------------------------------------------------------------------------------------------

void MainWindow::setupCalibrationTab()
{
  ui->m_QTreeWidget_CalibrationPoints->clear();

  QList<QPointF> calibrationPoints = m_temperatureSensor->getCalibrationPoints();

  foreach (QPointF point, calibrationPoints)
  {
    QTreeWidgetItem *qTreeWidgetItem = new QTreeWidgetItem(ui->m_QTreeWidget_CalibrationPoints);
    qTreeWidgetItem->setText(0, QString::number(point.x()));
    qTreeWidgetItem->setText(1, QString::number(point.y()));
  }
}

//-----------------------------------------------------------------------------------------------------------------------------

