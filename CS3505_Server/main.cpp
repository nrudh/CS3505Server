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

using namespace std;

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("73.3.77.26");
    db.setPort(3506);
    db.setDatabaseName("thegeographers");
    db.setUserName("student");
    db.setPassword("CS3505");
    bool ok = db.open();

    //qDebug() << "drivers: "<< QSqlDatabase::drivers();

    if (ok)
    {
        cout << "Connected!" << endl;
        QSqlQuery query(db);
        if(query.exec("SELECT * from test"))
        {
           cout << "query executed" << endl;

           while(query.next()) // switch this back to while
           {
                   QString username = query.value(0).toString();
                   QString password = query.value(1).toString();
                   //qDebug() << username << " " << password << endl;
                   cout << username.toStdString() << " " << password.toStdString() << endl;
           }
        }
        else
             cout << "query failed!" << endl;

        QSqlQuery query1(db);
        query1.prepare("INSERT INTO test (users, password) "
                      "VALUES (:users, :password)");
        query1.bindValue(":users", "Court");
        query1.bindValue(":password", "21248");
        if(query1.exec())
            cout << "inserted: " << query1.numRowsAffected() << endl;
        else
            cout << "insert failed!" << endl;

        QSqlQuery query2(db);
        if(query2.exec("delete from test where users = 'Court'"))
        {
            cout << "deleted: " << query2.numRowsAffected() << endl;
        }
        else
            cout << "delete failed!" << endl;

       db.close();
   }
   else
       cout << "Failure!";

    //create server for tests
    MyServer server;
    server.startServer();

    //create client for test
    MySocket socket;
    socket.doConnect();

    return a.exec();
    //return a.exec();
}
