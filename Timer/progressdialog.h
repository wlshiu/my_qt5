#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(QWidget *parent = nullptr);
    ~ProgressDialog();

    void setTimerSart();

public slots:
    void updatePopProcessBar();


private slots:
    void on_btnExit_clicked();

private:
    Ui::ProgressDialog  *ui;
    QTimer              *m_timer;

};

#endif // PROGRESSDIALOG_H
