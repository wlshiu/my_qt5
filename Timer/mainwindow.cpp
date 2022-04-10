#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "progressdialog.h"

#include <QTimer>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);

    hasPopProgressBar = false;

    popProgress = new ProgressDialog(this);

    m_timer = new QTimer(this);

    ui->progressBar->setRange(0, 999);
    ui->progressBar->setValue(0);

    connect(ui->btnStart, SIGNAL(clicked()), this, SLOT(on_btnStart_clicked()));
    connect(ui->btnStop, SIGNAL(clicked()), this, SLOT(on_btnStop_clicked()));

    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateProcessBar()));


    return;
}

MainWindow::~MainWindow()
{
    delete m_timer;
    delete popProgress;
    delete ui;
}

void MainWindow::updateProcessBar()
{
    int CurrentValue = ui->progressBar->value();

    if( CurrentValue == 1000 )
    {
        m_timer->stop();
        return;
    }

    CurrentValue++;

    qDebug() << CurrentValue;

    ui->progressBar->setValue(CurrentValue);
}


void MainWindow::on_btnStart_clicked()
{
    if( hasPopProgressBar == false )
    {
        ui->progressBar->setValue(0);
        m_timer->start(10); // 100 ms
    }
    else
    {
        popProgress->setTimerSart();
    }
}

void MainWindow::on_btnStop_clicked()
{
    m_timer->stop();
}

void MainWindow::on_checkBox_stateChanged(int state)
{
    if( state == Qt::Checked )
    {
        hasPopProgressBar = true;
        connect(ui->btnStart, SIGNAL(clicked()), popProgress, SLOT(show()));
    }
    else
    {
        hasPopProgressBar = false;
        disconnect(ui->btnStart, SIGNAL(clicked()), popProgress, SLOT(show()));
//        connect(ui->btnStart, SIGNAL(clicked()), this, SLOT(on_btnStart_clicked()));
    }
    return;
}
