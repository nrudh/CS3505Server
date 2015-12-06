#include "MySocket.h"

MySocket::MySocket(QObject *parent) : QObject(parent)
{
}

void MySocket::doConnect()
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    qDebug() << "connecting...";

    // this is not blocking call
//    socket->connectToHost("google.com", 80);
    socket->connectToHost("localhost", 1234);

    // we need to wait...
    if(!socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << socket->errorString();
    }
}

void MySocket::connected()
{
    qDebug() << "connected...";
}

void MySocket::disconnected()
{
    qDebug() << "disconnected...";
}

void MySocket::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void MySocket::readyRead()
{
    qDebug() << "reading...";

    // read the data from the socket
    qDebug() << socket->readAll();
}

qint64 MySocket::write(char *data)
{
    qint64 hold = socket->write(data);
    socket->waitForBytesWritten();
    return hold;
}
