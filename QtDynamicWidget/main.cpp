#include "qtdynamicwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtDynamicWidget w;
    w.show();
    return a.exec();
}
