#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
//#include <QTcpSocket>
//#include <QAbstractSocket>
#include <QDebug>

class MySocket : public QObject
{
    Q_OBJECT
public:
    explicit MySocket(QObject *parent = 0);

    void doConnect();
public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    qint64 write(char *data);

private:
    QTcpSocket *socket;
};

#endif // MYSOCKET_H
