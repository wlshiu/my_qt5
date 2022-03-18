#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString     filePath = QFileDialog::getOpenFileName(this, tr("Open"),
                                                      QDir::homePath(),
                                                      tr("*.bin"));
    if( filePath.isEmpty() )
        return;

    ui->lineEdit->setText(filePath);
    return;
}


void MainWindow::on_setOutDir_clicked()
{
    QString     dirPath = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                QDir::homePath(),
                                                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if( dirPath.isEmpty() )
        return;

    ui->OutDirPath->setText(dirPath);
    return;
}

