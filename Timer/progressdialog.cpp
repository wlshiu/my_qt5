#include "progressdialog.h"
#include "ui_progressdialog.h"

#include <QtDebug>

ProgressDialog::ProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);

    ui->popProgressBar->setValue(0);
    ui->popProgressBar->setRange(0, 999);
    ui->popProgressBar->setValue(0);

    m_timer = new QTimer;

    connect(m_timer, SIGNAL(timeout()), this, SLOT(updatePopProcessBar()));


    return;
}

ProgressDialog::~ProgressDialog()
{
    delete m_timer;
    delete ui;
}

void ProgressDialog::setTimerSart()
{
    ui->popProgressBar->setValue(0);
    m_timer->start(10); // 100 ms
}

void ProgressDialog::updatePopProcessBar()
{
    int CurrentValue = ui->popProgressBar->value();

    if( CurrentValue == 1000 )
    {
        m_timer->stop();
        return;
    }

    CurrentValue++;

    qDebug() << CurrentValue;

    ui->popProgressBar->setValue(CurrentValue);
}

void ProgressDialog::on_btnExit_clicked()
{
    m_timer->stop();
    hide();
}
