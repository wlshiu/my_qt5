

#include "console.h"
#include <QScrollBar>
#include <QtCore/QDebug>

Console::Console(QWidget *parent)
    : QPlainTextEdit(parent)
    , isLocalEchoEnabled(false)
{
    document()->setMaximumBlockCount(1000);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::gray);
    setPalette(p);

    QFont font = document()->defaultFont();
    font.setFamily("consolas");
    font.setPointSize(12);
    document()->setDefaultFont(font);
}

void Console::putData(const QByteArray &data)
{
    insertPlainText(QString(data));

    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

void Console::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_Backspace:
            qDebug("Press %s ...", "Backspace");
            break;
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_Up:
        case Qt::Key_Down:
            break;
        default:
            if (isLocalEchoEnabled)
                QPlainTextEdit::keyPressEvent(e);
            emit getData(e->text().toLocal8Bit());
    }
}

void Console::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    setFocus();
}

void Console::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
}

void Console::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}
