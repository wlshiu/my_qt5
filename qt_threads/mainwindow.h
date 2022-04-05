#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QtCore>
#include <QQueue>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ThreadImpl: public QObject
{
    Q_OBJECT

public:
    ThreadImpl(QObject* parent = nullptr);
    ~ThreadImpl();

signals:
    void response(long *pPool);

public slots:
    void Routine(long *dataPool);
//    void request(long *dataPool);

private:
    long    *pPool;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void request(long *dataPool);

public slots:
    void response(long *pPool);

private slots:
    void on_btnRead_clicked();

    void on_btnWrite_clicked();

private:
    Ui::MainWindow *ui;

    QThread        *myThread;

    ThreadImpl     threadImpl;

    long           *dataPool;

};



#endif // MAINWINDOW_H
