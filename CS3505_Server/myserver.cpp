// MyServer.cpp

#include "myserver.h"
#include "myrunnable.h"

MyServer::MyServer(QObject *parent) : QTcpServer(parent)
{
    pool = new QThreadPool(this);
    pool->setMaxThreadCount(5);
    db = new DataBase();
}

void MyServer::startServer()
{
    if(this->listen(QHostAddress::Any, 1234))
    {
        qDebug() << "Server started";
    }
    else
    {
        qDebug() << "Server did not start!";
    }
}

void MyServer::incomingConnection(qintptr handle)
{
    MyRunnable *task = new MyRunnable(db);
    task->setAutoDelete(true);

    task->socketDescriptor = handle;

    pool->start(task);
    qDebug() << "pool started";
}
