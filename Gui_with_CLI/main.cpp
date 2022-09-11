#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>


static QString          g_Input;

static void
_usage(QCoreApplication &app)
{
    printf("usage: %s <options>\n"
           "options:\n"
           "  --input      The input file\n"
           "\n",
           qPrintable(app.applicationName()));
    exit(-1);
    return;
}

static int
_parse_param(QCoreApplication &app)
{
    int     rval = 0;

    if( app.arguments().size() < 2 )
    {
        _usage(app);
        return -1;
    }

    g_Input.clear();

    for(int i = 1; i < app.arguments().size(); i++)
    {
        QString     option = app.arguments().at(i);

        if( !option.compare("--input", Qt::CaseInsensitive) )
        {
            QString    value = app.arguments().at(i + 1);
            g_Input.append(value);

            i++;
        }
    }

    do {
        if( g_Input.isEmpty() == true )
        {
            rval = -__LINE__;
            break;
        }
    } while(0);

    return rval;
}


int main(int argc, char *argv[])
{
    if( argc == 1 )
    {
        QApplication    a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }
    else
    {
        int     rval = 0;
        QCoreApplication    a(argc, argv);
        if( (rval = _parse_param(a)) != 0 )
        {
            printf("Wrong arguments !\n");
            return rval;
        }

        printf("input : %s\n", qPrintable(g_Input));
        return rval;
    }
    return 0;
}
