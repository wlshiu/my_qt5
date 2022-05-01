#ifndef XMODEM_H
#define XMODEM_H

#include <QObject>

class Xmodem : public QObject
{
    Q_OBJECT
public:
    explicit Xmodem(QObject *parent = nullptr);
    ~Xmodem();

signals:

};

#endif // XMODEM_H
