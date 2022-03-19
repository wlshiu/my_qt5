#include "qtdynamicwidget.h"
#include "ui_qtdynamicwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>

QtDynamicWidget::QtDynamicWidget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QtDynamicWidget)
{
    ui->setupUi(this);

    connect(ui->addWidget_button, SIGNAL(clicked()), this, SLOT(on_addWidget_button_clicked()));
}

QtDynamicWidget::~QtDynamicWidget()
{
    delete ui;
}


void QtDynamicWidget::on_addWidget_button_clicked()
{
    QVBoxLayout     *layout = qobject_cast<QVBoxLayout*>(ui->widget_frame->layout());
    if (layout == nullptr)
    {
        return;
    }

    QHBoxLayout     *newLayout = new QHBoxLayout(ui->widget_frame->widget());

    QString buttonText = tr("Button: #%1").arg(layout->count());

    QPushButton     *button = new QPushButton(buttonText, ui->widget_frame->widget());
//    layout->insertWidget(0, button);
    newLayout->addWidget(button);

    mButtonToLayoutMap.insert(button, newLayout);

    QCheckBox   *checkBox = new QCheckBox("Check me!", ui->widget_frame->widget());
    newLayout->addWidget(checkBox);

    layout->insertLayout(0, newLayout);

    connect(button, SIGNAL(clicked()), this, SLOT(onRemoveWidget()));
}

void QtDynamicWidget::onRemoveWidget()
{
    /**
     *  sender() report which object send signal
     */
    QPushButton     *button = qobject_cast<QPushButton*>(sender());
    QHBoxLayout     *layout = mButtonToLayoutMap.value(button);

    while (layout->count() != 0)
    {
        QLayoutItem* item = layout->takeAt(0);

        if (item)
        {
            delete item->widget();
            delete item;
        }
    }
    delete layout;
}
