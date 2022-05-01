#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);

    pSerial = new QSerialPort(this);

    pSettings = new struct SerailPortSettings;

    connect(pSerial, SIGNAL(error(QSerialPort::SerialPortError)),
            this, SLOT(handleSerialPortError(QSerialPort::SerialPortError)));
    connect(pSerial, SIGNAL(readyRead()), this, SLOT(readData()));

    connect(ui->PortBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showPortInfo(int)));


    fillPortsParameters();
    fillPortsInfo();
    updateSettings();


    return;
}

MainWindow::~MainWindow()
{
    delete pSerial;
    delete pSettings;
    delete ui;
}

void MainWindow::fillPortsInfo()
{
    ui->PortBox->clear();

    static const QString    blankString = QObject::tr("N/A");
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

        ui->PortBox->addItem(list.first(), list);
    }
}

void MainWindow::updateSettings()
{
    pSettings->portName = ui->PortBox->currentText();

    if (ui->baudRateBox->currentIndex() == 4) {
        pSettings->baudRate = ui->baudRateBox->currentText().toInt();
    } else {
        pSettings->baudRate = static_cast<QSerialPort::BaudRate>(
                                ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    }
    pSettings->stringBaudRate = QString::number(pSettings->baudRate);

    pSettings->dataBits = static_cast<QSerialPort::DataBits>(
                                ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
    pSettings->strDataBits = ui->dataBitsBox->currentText();

    pSettings->parity = static_cast<QSerialPort::Parity>(
                                ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
    pSettings->strParity = ui->parityBox->currentText();

    pSettings->stopBits = static_cast<QSerialPort::StopBits>(
                                ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
    pSettings->strStopBits = ui->stopBitsBox->currentText();

    pSettings->flowControl = static_cast<QSerialPort::FlowControl>(
                                ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
    pSettings->strFlowControl = ui->flowControlBox->currentText();

    return;
}

void MainWindow::showPortInfo(int idx)
{
    if (idx != -1)
    {
        QStringList list = ui->PortBox->itemData(idx).toStringList();
        ui->DescriptionLabel->setText(tr("Description: %1").arg(list.at(1)));
        ui->ManufacturerLabel->setText(tr("Manufacturer: %1").arg(list.at(2)));
        ui->SerialNumberLabel->setText(tr("Serial number: %1").arg(list.at(3)));
        ui->LocationLabel->setText(tr("Location: %1").arg(list.at(4)));
        ui->vidLabel->setText(tr("Vendor Identifier: %1").arg(list.at(5)));
        ui->pidLabel->setText(tr("Product Identifier: %1").arg(list.at(6)));

        updateSettings();
    }
    else
    {
        ui->DescriptionLabel->setText(tr("Description:"));
        ui->ManufacturerLabel->setText(tr("Manufacturer: "));
        ui->SerialNumberLabel->setText(tr("Serial number: "));
        ui->LocationLabel->setText(tr("Location: "));
        ui->vidLabel->setText(tr("Vendor Identifier: "));
        ui->pidLabel->setText(tr("Product Identifier: "));
    }
    return;
}

void MainWindow::lockSettings(bool isLock)
{
    ui->PortBox->setEnabled(!isLock);

    ui->baudRateBox->setEnabled(!isLock);
    ui->dataBitsBox->setEnabled(!isLock);
    ui->parityBox->setEnabled(!isLock);
    ui->stopBitsBox->setEnabled(!isLock);
    ui->flowControlBox->setEnabled(!isLock);
    return;
}

void MainWindow::handleSerialPortError(QSerialPort::SerialPortError error)
{
    if( error == QSerialPort::ResourceError )
    {
        QMessageBox::critical(this, tr("Critical Error"), pSerial->errorString());
        pSerial->close();
        lockSettings(false);
        ui->btn_OpenPort->setText("Open Port");
    }
    return;
}


void MainWindow::on_btn_OpenFile_clicked()
{
    QString     filePath = QFileDialog::getOpenFileName(this, tr("Open"),
                                                        QDir::currentPath(),
                                                        tr("*.*"));
    if( filePath.isEmpty() )
        return;

    ui->txt_FilePath->setText(filePath);
    return;
}

void MainWindow::fillPortsParameters()
{
    //Filling QCombo Box for choosing BaudRate
    ui->baudRateBox->addItem("9600",QSerialPort::Baud9600);
    ui->baudRateBox->addItem("38400",QSerialPort::Baud38400);
    ui->baudRateBox->addItem("57600",QSerialPort::Baud57600);
    ui->baudRateBox->addItem("115200",QSerialPort::Baud115200);
    ui->baudRateBox->addItem("Custom");

    //Filling QCombo Box for choosing Data Bits
    ui->dataBitsBox->addItem("8",QSerialPort::Data8);
    ui->dataBitsBox->addItem("7",QSerialPort::Data7);
    ui->dataBitsBox->addItem("6",QSerialPort::Data6);
    ui->dataBitsBox->addItem("5",QSerialPort::Data5);

    //Filling QCombo Box for choosing Parity
    ui->parityBox->addItem("None",QSerialPort::NoParity);
    ui->parityBox->addItem("Even",QSerialPort::EvenParity);
    ui->parityBox->addItem("Odd",QSerialPort::OddParity);
    ui->parityBox->addItem("Mark",QSerialPort::MarkParity);
    ui->parityBox->addItem("Space",QSerialPort::SpaceParity);

    //Filling QCombo Box for choosing the number of stop bits
    ui->stopBitsBox->addItem("1",QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem("1.5",QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem("2",QSerialPort::TwoStop);

    //Filling QCombo Box for choosing Flow control
    ui->flowControlBox->addItem("None",QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem("RTS/CTS",QSerialPort::HardwareControl);
    ui->flowControlBox->addItem("XON/XOFF",QSerialPort::SoftwareControl);
}

void MainWindow::on_btn_OpenPort_clicked()
{
    if( ui->btn_OpenPort->text() == "Open Port" )
    {
        updateSettings();

        pSerial->setPortName(pSettings->portName);
        pSerial->setBaudRate(pSettings->baudRate);
        pSerial->setDataBits(pSettings->dataBits);
        pSerial->setParity(pSettings->parity);
        pSerial->setStopBits(pSettings->stopBits);
        pSerial->setFlowControl(pSettings->flowControl);

        if( pSerial->open(QIODevice::ReadWrite) )
        {
            showPortInfo(ui->PortBox->currentIndex());
            lockSettings(true);
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), pSerial->errorString());
        }

        ui->btn_OpenPort->setText("Close Port");
    }
    else
    {
        pSerial->close();
        showPortInfo(-1);
        lockSettings(false);
        ui->btn_OpenPort->setText("Open Port");
    }


    return;
}
