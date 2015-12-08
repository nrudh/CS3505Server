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
    //socket.doConnect();
    //socket.write("login McKay 12345");
    //socket.write("update Scott 10101 3 99");
    //socket.write("scores McKay 12345;scores Nick 56789;");
    //socket.write("display");
    //socket.write("adduser Amelia 22222");
    //socket.write("delete Amelia 22222");

    //socket2.doConnect();
    //socket2.write("update McKay 12345 2 12");

    return a.exec();
}
