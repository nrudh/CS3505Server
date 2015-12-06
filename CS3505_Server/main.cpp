#include <vector>
#include <string>
#include <QDebug>
#include <iostream>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QtSql>
#include "myrunnable.h"
#include "mysocket.h"
#include "myserver.h"
#include "database.h"

using namespace std;

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    //create server for tests
    MyServer server;
    server.startServer();

    //create client for test
    MySocket socket,socket2;
    socket.doConnect();
    socket.write("update Nick 56789 2 10");

    socket2.doConnect();
    socket2.write("scores McKay 12345");

    return a.exec();
}
