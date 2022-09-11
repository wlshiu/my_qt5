
#include <stdio.h>
#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>

static QString          g_Input;
static QString          g_Output;

static void
_usage(QCoreApplication &app)
{
    printf("usage: %s <options>\n"
           "options:\n"
           "  --input      The input file\n"
           "  --output     The output file\n"
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
    g_Output.clear();

    for(int i = 1; i < app.arguments().size(); i++)
    {
        QString     option = app.arguments().at(i);

        if( !option.compare("--input", Qt::CaseInsensitive) )
        {
            QString    value = app.arguments().at(i + 1);
            g_Input.append(value);

            i++;
        }
        else if( !option.compare("--output", Qt::CaseInsensitive) )
        {
            QString    value = app.arguments().at(i + 1);
            g_Output.append(value);

            i++;
        }
    }

    do {
        if( g_Input.isEmpty() == true || g_Output.isEmpty() == true )
        {
            rval = -__LINE__;
            break;
        }
    } while(0);

    return rval;
}


int main(int argc, char *argv[])
{
    int                 rval = 0;
    QCoreApplication    a(argc, argv);

    if( (rval = _parse_param(a)) != 0 )
    {
        printf("Wrong arguments !\n");
        return rval;
    }

    printf("input : %s\n", qPrintable(g_Input));
    printf("output: %s\n", qPrintable(g_Output));
    printf("done~~\n");
    return 0;
}
