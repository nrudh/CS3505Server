// MyServer.h

#ifndef MYSERVER_H
#define MYSERVER_H

#include <QtNetwork/QTcpServer>
//#include <QTcpServer>
#include <QThreadPool>
#include <QDebug>

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = 0);
    void startServer();

protected:
    void incomingConnection( qintptr handle );

signals:

public slots:

private:
    QThreadPool *pool;
};

#endif // MYSERVER_H
