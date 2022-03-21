#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QDebug>



MyThread::MyThread()
    : is_exiting(false)
{
    return;
}

MyThread::~MyThread()
{
    return;
}

void MyThread::run()
{
    quint32     cnt = 0;
    qDebug("button clicked.");

    while ( is_exiting == false )
    {
        this->sleep(1);

        qDebug()<< this->name << " Running " << cnt++ << endl;
    }
}

void MyThread::EnterThread()
{
   return;
}

void MyThread::stop()
{
    is_exiting = true;
    wait();
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete my_thread;
}


void MainWindow::on_pushButton_clicked()
{
//    if( this->my_thread )
    {
        my_thread = new MyThread();

        my_thread->name = "my-thread";

        connect(my_thread, &QThread::started, 0, &MyThread::EnterThread);

        my_thread->start();
    }
}
