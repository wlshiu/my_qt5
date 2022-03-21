#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QtCore>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MyThread: public QThread
{
    Q_OBJECT
public:
    MyThread();
    ~MyThread();

    virtual void run() override;

    void EnterThread();
    void stop();

    QString     name;

private:

    Ui::MainWindow *ui;

    bool        is_exiting;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    MyThread        *my_thread;
};
#endif // MAINWINDOW_H
