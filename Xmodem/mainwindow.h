#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include  <QtSerialPort/QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    struct SerailPortSettings {
        QString                 portName;
        qint32                  baudRate;
        QString                 stringBaudRate;
        QSerialPort::DataBits   dataBits;
        QString                 strDataBits;
        QSerialPort::Parity     parity;
        QString                 strParity;
        QSerialPort::StopBits   stopBits;
        QString                 strStopBits;
        QSerialPort::FlowControl flowControl;
        QString                 strFlowControl;
    };

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_btn_OpenFile_clicked();

    void on_btn_OpenPort_clicked();

private:
    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();
    void showPortInfo(int idx);
    void lockSettings(bool isLock);

    void handleSerialPortError(QSerialPort::SerialPortError error);

private:
    Ui::MainWindow  *ui;
    QSerialPort     *pSerial;
    struct SerailPortSettings  *pSettings;
};
#endif // MAINWINDOW_H
