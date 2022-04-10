#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);

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
    ui->progressBar->setValue(0);
    m_timer->start(10); // 100 ms
}

void MainWindow::on_btnStop_clicked()
{
    m_timer->stop();
}
