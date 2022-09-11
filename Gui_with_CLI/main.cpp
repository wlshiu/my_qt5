#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <windows.h>

static QString          g_Input;


static void
_usage(QCoreApplication &app)
{
    qInfo("usage: %s <options>\n"
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

static void _Show_Console()
{
    AllocConsole();

//    FILE    *pFileCon = freopen("CONOUT$", "w", stdout);
//    COORD   coordInfo;

//    (void)pFileCon;
//    coordInfo.X = 80;
//    coordInfo.Y = 900;

//    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coordInfo);
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);
    return;
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

        _Show_Console();

        if( (rval = _parse_param(a)) != 0 )
        {
            qInfo("Wrong arguments !\n");
            return rval;
        }

        qInfo("input : %s\n", qPrintable(g_Input));
        return rval;
    }
    return 0;
}
