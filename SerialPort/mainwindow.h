#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Console;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    struct SerialPort_Settings {
        QString                 name;
        qint32                  baudRate;
        QString                 stringBaudRate;
        QSerialPort::DataBits   dataBits;
        QString                 stringDataBits;
        QSerialPort::Parity     parity;
        QString                 stringParity;
        QSerialPort::StopBits   stopBits;
        QString                 stringStopBits;
    };

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_OpenPort_clicked();

    void lockSettings(bool isLock);
    void fillPortsParameters();
    void updatePortSettings();
    void fillPortsInfo();
    void showPortInfo(int idx);

    void sendData();
    void readData();
    void writeData(const QByteArray &data);

    void closeSerialPort();
    void handleError(QSerialPort::SerialPortError error);

    void on_btn_CmdSend_clicked();

private:
    Ui::MainWindow          *ui;
    QSerialPort             *serial;
    Console                 *console;

    struct SerialPort_Settings    *settings;
};
#endif // MAINWINDOW_H
