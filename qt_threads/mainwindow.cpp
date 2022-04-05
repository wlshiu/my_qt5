#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QObject>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myThread = new QThread;

    threadImpl.moveToThread(myThread);

    myThread->start();

    dataPool = new long[512];
    memset(dataPool, 0x0, sizeof(long)*512);


    QObject::connect(&threadImpl, SIGNAL(response(long *)), this, SLOT(response(long *)));
    QObject::connect(this, SIGNAL(request(long *)), &threadImpl, SLOT(Routine(long *)));

    return;
}

MainWindow::~MainWindow()
{
    delete ui;

    delete[] dataPool;

    myThread->wait();
    delete myThread;
}

void MainWindow::response(long *pPool)
{
    QString     str = QString::number(pPool[0]); // str2int
    ui->Line_11->setText(str);
    return;
}

void MainWindow::on_btnRead_clicked()
{
    QString     str = ui->Line_10->text();
    bool        ok;

    if( str.indexOf('x', Qt::CaseInsensitive) == -1 )
        dataPool[0] = str.toInt(&ok, 10);
    else
        dataPool[0] = str.toInt(&ok, 16);

    emit request(dataPool);
    return;
}

void MainWindow::on_btnWrite_clicked()
{
    QString     str_1 = ui->Line_20->text();
    QString     str_2 = ui->Line_21->text();
    bool        ok;

    if( str_1.indexOf('x', Qt::CaseInsensitive) == -1 )
        dataPool[0] = str_1.toInt(&ok, 10);
    else
        dataPool[0] = str_1.toInt(&ok, 16);

    if( str_2.indexOf('x', Qt::CaseInsensitive) == -1 )
        dataPool[1] = str_2.toInt(&ok, 10);
    else
        dataPool[1] = str_2.toInt(&ok, 16);

    emit request(dataPool);

    return;
}


ThreadImpl::ThreadImpl(QObject *parent)
    : QObject(parent)
{
    pPool = new long[1024];
}

ThreadImpl::~ThreadImpl()
{
    delete[] pPool;
}

void ThreadImpl::Routine(long *dataPool)
{
    qDebug() << "Routine():" << QThread::currentThreadId()
             << ", Get:" << dataPool[0]
             << ", Get:" << dataPool[1];

    pPool[0] = dataPool[0];
    emit response(pPool);
}



