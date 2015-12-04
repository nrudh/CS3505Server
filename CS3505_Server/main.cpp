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
    //db.setHostName("73.3.77.26");
    db.setHostName("192.168.1.118");
    db.setPort(3506);
    db.setDatabaseName("thegeographers");
    db.setUserName("student");
    db.setPassword("CS3505");
    bool ok = db.open();

    //qDebug() << "drivers: "<< QSqlDatabase::drivers();

    if (ok)
    {
        cout << "Connected to database!" << endl << endl;
        QSqlQuery query(db);

// Select * query
        if(query.exec("SELECT * from User_table"))
        {
           cout << "Contents of 'User_table' table" << endl;

           while(query.next())
           {
                   QString id = query.value(0).toString();
                   QString user = query.value(1).toString();
                   QString password = query.value(2).toString();
                   //qDebug() << id << " " << username << " " << password << endl;
                   cout << id.toStdString() << " " << user.toStdString() << " " << password.toStdString() << endl;
           }
        }
        else
             cout << "Query failed!" << endl;

// Insert into test query
        //QSqlQuery query1(db);
        QString sent_user = "Court";
        QString sent_password = "000";
        query.prepare("INSERT INTO User_table (user, password) "
                      "VALUES (:user, :password)");
        query.bindValue(":user", sent_user);
        query.bindValue(":password", sent_password);
        if(query.exec())
            cout << endl << "Inserted user: " << sent_user.toStdString() << " " << query.numRowsAffected() << endl << endl;
        else
            cout << "Insert failed!" << endl;

// Query if user and password exists
        query.prepare("SELECT * FROM User_table WHERE user = '" + sent_user + "' AND password = '" + sent_password + "'");
        if(query.exec())
            cout << "Found user: " << sent_user.toStdString() << " " <<query.size() << endl << endl;
        else
            cout << "Query failed!" << endl;

// Query to delete a user
        if(query.exec("DELETE FROM User_table WHERE user = '" + sent_user + "'"))
        {
            cout << "Deleted user: " << sent_user.toStdString() << " " << query.numRowsAffected() << endl << endl;
        }
        else
            cout << "Delete failed!" << endl;

// Query to get the state of a user
        sent_user = "McKay";
        sent_password = "12345";
        QString id;
        if(query.exec("SELECT id FROM User_table WHERE user = '" + sent_user + "' AND password = '" + sent_password + "'"))
        {
            query.first();
            id = query.value(0).toString();
        }
        if(query.exec("SELECT score FROM level1_table where id = " + id))
        {
            query.first();
            cout << "user " << sent_user.toStdString() << " level 1 score: " << query.value(0).toString().toStdString() << endl;
        }
        if(query.exec("SELECT score FROM level2_table where id = " + id))
        {
            query.first();
            cout << "user " << sent_user.toStdString() << " level 2 score: " << query.value(0).toString().toStdString() << endl;
        }
        if(query.exec("SELECT score FROM level3_table where id = " + id))
        {
            query.first();
            cout << "user " << sent_user.toStdString() << " level 3 score: " << query.value(0).toString().toStdString() << endl << endl;
        }

// Query to update a level score
        sent_user = "McKay";
        sent_password = "12345";
        QVariant var_id;
        int score = 3;
        int level = 3;
        if(query.exec("SELECT id FROM User_table WHERE user = '" + sent_user + "' AND password = '" + sent_password + "'"))
        {
            query.first();
            var_id = query.value(0);
        }
        //query.prepare("INSERT INTO Level" + QString::number(level) + "_Table (id, score) VALUES (:id, :score)");
        if(query.exec("REPLACE INTO level" + QString::number(level) + "_Table SET id = " + var_id.toString() + ", score = " + QString::number(score)))
        {
            cout << "Inserted score :" << score << " for user: " << sent_user.toStdString() << " into Level" << std::to_string(level) << "_Table" << endl << endl;
        }
        else
        {
            cout << "level update failed!" << endl;
            cout << query.lastQuery().toStdString() << endl;
        }

        db.close();
   }
   else
       cout << "Failure!" << endl;


       db.close();

    //create server for tests
    MyServer server;
    server.startServer();

    //create client for test
    MySocket socket;
    socket.doConnect();

    return a.exec();
    //return a.exec();
}
