#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QScrollBar>
#include <QPlainTextEdit>

#include "console.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial = new QSerialPort(this);

    settings = new struct SerialPort_Settings;

    txt_CmdSendHistory = QStringList();

    /* Replace QObject 'txt_Console' with QObject 'Console' */
    console = new Console(ui->txt_Console->parentWidget());
    console->setEnabled(true);
    console->setReadOnly(false);

    QPlainTextEdit      *txt_Console_org = ui->txt_Console;

    ui->txt_Console->parentWidget()->layout()->replaceWidget(ui->txt_Console, console);
    ui->txt_Console = console;
    delete txt_Console_org;  // delete original QObject 'txt_Console'

    /* configurate signal and slot */
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

    connect(ui->comboBox_PortName, SIGNAL(currentIndexChanged(int)), this, SLOT(showPortInfo(int)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    connect(console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));


    fillPortsParameters();
    updatePortSettings();
    fillPortsInfo();

    return;
}

MainWindow::~MainWindow()
{
    delete serial;
    delete settings;
    delete ui;
}

void MainWindow::closeSerialPort()
{
    serial->close();
    return;
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}


void MainWindow::lockSettings(bool isLock)
{
    ui->comboBox_PortName->setEnabled(!isLock);

    ui->comboBox_BaudRate->setEnabled(!isLock);
    ui->comboBox_DataBits->setEnabled(!isLock);
    ui->comboBox_Parity->setEnabled(!isLock);
    ui->comboBox_StopBits->setEnabled(!isLock);

}

void MainWindow::fillPortsParameters()
{
    /* Filling QCombo Box for choosing BaudRate */
    ui->comboBox_BaudRate->addItem("9600",QSerialPort::Baud9600);
    ui->comboBox_BaudRate->addItem("38400",QSerialPort::Baud38400);
    ui->comboBox_BaudRate->addItem("57600",QSerialPort::Baud57600);
    ui->comboBox_BaudRate->addItem("115200",QSerialPort::Baud115200);


    /* Filling QCombo Box for choosing Data Bits */
    ui->comboBox_DataBits->addItem("8",QSerialPort::Data8);
//    ui->comboBox_DataBits->addItem("7",QSerialPort::Data7);
//    ui->comboBox_DataBits->addItem("6",QSerialPort::Data6);
//    ui->comboBox_DataBits->addItem("5",QSerialPort::Data5);

    /* Filling QCombo Box for choosing Parity */
    ui->comboBox_Parity->addItem("None",QSerialPort::NoParity);
    ui->comboBox_Parity->addItem("Even",QSerialPort::EvenParity);
    ui->comboBox_Parity->addItem("Odd",QSerialPort::OddParity);

    /* Filling QCombo Box for choosing the number of stop bits */
    ui->comboBox_StopBits->addItem("1",QSerialPort::OneStop);
//    ui->comboBox_StopBits->addItem("2",QSerialPort::TwoStop);

    return;
}

void MainWindow::fillPortsInfo()
{
    ui->comboBox_PortName->clear();
    static const QString blankString = QObject::tr("N/A");
    QString description;
    QString manufacturer;
    QString serialNumber;

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->comboBox_PortName->addItem(list.first(), list);
    }
}


void MainWindow::updatePortSettings()
{
    settings->name = ui->comboBox_PortName->currentText();

    if (ui->comboBox_BaudRate->currentIndex() == 4) {
        settings->baudRate = ui->comboBox_BaudRate->currentText().toInt();
    } else {
        settings->baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->comboBox_BaudRate->itemData(ui->comboBox_BaudRate->currentIndex()).toInt());
    }
    settings->stringBaudRate = QString::number(settings->baudRate);

    settings->dataBits = static_cast<QSerialPort::DataBits>(
                ui->comboBox_DataBits->itemData(ui->comboBox_DataBits->currentIndex()).toInt());
    settings->stringDataBits = ui->comboBox_DataBits->currentText();

    settings->parity = static_cast<QSerialPort::Parity>(
                ui->comboBox_Parity->itemData(ui->comboBox_Parity->currentIndex()).toInt());
    settings->stringParity = ui->comboBox_Parity->currentText();

    settings->stopBits = static_cast<QSerialPort::StopBits>(
                ui->comboBox_StopBits->itemData(ui->comboBox_StopBits->currentIndex()).toInt());
    settings->stringStopBits = ui->comboBox_StopBits->currentText();

    return;
}

void MainWindow::showPortInfo(int idx)
{
    if (idx != -1)
    {
        QStringList list = ui->comboBox_PortName->itemData(idx).toStringList();
        ui->label_Description->setText(tr("Description: %1").arg(list.at(1)));
        ui->label_Manufacturer->setText(tr("Manufacturer: %1").arg(list.at(2)));
//        ui->SerialNumberLabel->setText(tr("Serial number: %1").arg(list.at(3)));
//        ui->LocationLabel->setText(tr("Location: %1").arg(list.at(4)));
        ui->label_VendorID->setText(tr("Vendor Identifier: %1").arg(list.at(5)));
        ui->label_ProductID->setText(tr("Product Identifier: %1").arg(list.at(6)));

        updatePortSettings();
    }
}

void MainWindow::sendData()
{
    QString     str = ui->txt_CmdSend->currentText();
    if (str.isEmpty())
    {
        ui->txt_CmdSend->setFocus();
        return;
    }

    if (serial == 0 || !serial->isOpen())
    {
        return;
    }

    txt_CmdSendHistory.append(str);
    str.append('\n');

    serial->write(str.toLatin1().data());

    txt_CmdSendHistory.removeDuplicates();
    ui->txt_CmdSend->clear();
    ui->txt_CmdSend->addItems(txt_CmdSendHistory);

    ui->txt_CmdSend->setCurrentIndex(-1);
    ui->txt_CmdSend->setFocus();

    return;
}

void MainWindow::readData()
{
    QByteArray  data = serial->readAll();

    console->putData(data);
//    ui->txt_Console->insertPlainText(QString(data));

//    QScrollBar  *bar = ui->txt_Console->verticalScrollBar();
//    bar->setValue(bar->maximum());

    return;
}

void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void MainWindow::on_btn_OpenPort_clicked()
{
    if (ui->btn_OpenPort->text() == "Open Port")
    {
        updatePortSettings();
        serial->setPortName(settings->name);
        serial->setBaudRate(settings->baudRate);
        serial->setDataBits(settings->dataBits);
        serial->setParity(settings->parity);
        serial->setStopBits(settings->stopBits);

        if (serial->open(QIODevice::ReadWrite))
        {
            lockSettings(true);
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), serial->errorString());
        }

        ui->btn_OpenPort->setText("Close Port");

    }
    else
    {
        lockSettings(false);
        ui->btn_OpenPort->setText("Open Port");
    }
    return;
}

void MainWindow::on_btn_CmdSend_clicked()
{
    sendData();
    return;
}
