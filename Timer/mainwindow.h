#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ProgressDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void updateProcessBar();

private slots:
    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void on_checkBox_stateChanged(int state);

private:
    Ui::MainWindow *ui;
    ProgressDialog *popProgress;
    QTimer  *m_timer;
    bool    hasPopProgressBar;

};
#endif // MAINWINDOW_H
