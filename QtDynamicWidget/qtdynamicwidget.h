#ifndef QTDYNAMICWIDGET_H
#define QTDYNAMICWIDGET_H

#include <QMainWindow>

#include <QHash>
#include <QPushButton>
#include <QHBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class QtDynamicWidget; }
QT_END_NAMESPACE

class QtDynamicWidget : public QMainWindow
{
    Q_OBJECT

public:
    QtDynamicWidget(QWidget *parent = nullptr);
    ~QtDynamicWidget();

private slots:
    void on_addWidget_button_clicked();
    void onRemoveWidget();

private:
    Ui::QtDynamicWidget *ui;

    QHash<QPushButton*, QHBoxLayout*> mButtonToLayoutMap;
};
#endif // QTDYNAMICWIDGET_H
