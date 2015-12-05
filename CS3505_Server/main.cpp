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
void dispAllUsers(QSqlQuery);
bool addUser(string, string, QSqlQuery);
bool userExists(string, string, QSqlQuery);
bool deleteUser(string, string, QSqlQuery);
string getUserState(string, string, QSqlQuery);
bool updateScore(string, string, string, string, QSqlQuery);

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("73.3.77.26");
    //db.setHostName("192.168.1.118");
    db.setPort(3506);
    db.setDatabaseName("thegeographers");
    db.setUserName("student");
    db.setPassword("CS3505");
    //qDebug() << "drivers: "<< QSqlDatabase::drivers();
    if (db.open())
    {
        cout << "Connected to database!" << endl << endl;
        QSqlQuery query(db);

        //Queries
        dispAllUsers(query);

        string sent_user = "Court";
        string sent_password = "000";
        bool userAdded = addUser(sent_user, sent_password, query);

        bool userFound = userExists(sent_user, sent_password, query);

        bool deleted = deleteUser(sent_user, sent_password, query);

        sent_user = "McKay";
        sent_password = "12345";
        getUserState(sent_user, sent_password, query);

        sent_user = "McKay";
        sent_password = "12345";
        string score = "3";
        string level = "3";
        bool scoreUpdated = updateScore(sent_user, sent_password, score, level, query);

        db.close();
   }
   else
       cout << "Database Connection Failure!" << endl;

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

void dispAllUsers(QSqlQuery query)
{

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

}

bool addUser(string sent_user, string sent_password, QSqlQuery query)
{
    bool userAdded = false;
    query.prepare("INSERT INTO User_table (user, password) "
                  "VALUES (:user, :password)");
    query.bindValue(":user", QString::fromStdString(sent_user));
    query.bindValue(":password", QString::fromStdString(sent_password));
    if(query.exec())
    {
        cout << endl << "Inserted user: " << sent_user << " " << query.numRowsAffected() << endl << endl;
        if(query.size() > 0)
             userAdded = true;
    }
    else
        cout << "Insert failed!" << endl;
    return userAdded;
}

bool userExists(string sent_user, string sent_password, QSqlQuery query)
{
    // Query if user and password exists
    query.prepare("SELECT * FROM User_table WHERE user = '" + QString::fromStdString(sent_user) + "' AND password = '" + QString::fromStdString(sent_password) + "'");
    if(query.exec())
    {
        cout << "Found user: " << sent_user << " " <<query.size() << endl << endl;
        if(query.size() > 0)
            return true;
    }
    else
        cout << "Query failed!" << endl;

    return false;
}

bool deleteUser(string sent_user, string sent_password, QSqlQuery query)
{
    // Query to delete a user
    bool deleted = false;
    if(query.exec("DELETE FROM User_table WHERE user = '" + QString::fromStdString(sent_user) + "' AND password = '" + QString::fromStdString(sent_password) + "'"))
    {
        cout << "Deleted user: " << sent_user << " " << query.numRowsAffected() << endl << endl;
        if(query.size() > 0)
            deleted = true;
    }
    else
        cout << "Delete failed!" << endl;
    return deleted;
}

string getUserState(string sent_user, string sent_password, QSqlQuery query)
{
    // Query to get the state of a user
    QString id;
    string score1, score2, score3;
    if(query.exec("SELECT id FROM User_table WHERE user = '" + QString::fromStdString(sent_user) + "' AND password = '" + QString::fromStdString(sent_password) + "'"))
    {
        query.first();
        id = query.value(0).toString();
    }
    if(query.exec("SELECT score FROM level1_table where id = " + id))
    {
        query.first();
        score1 = query.value(0).toString().toStdString();
        cout << "user " << sent_user << " level 1 score: " << score1 << endl;
    }
    if(query.exec("SELECT score FROM level2_table where id = " + id))
    {
        query.first();
        score2 = query.value(0).toString().toStdString();
        cout << "user " << sent_user << " level 2 score: " << score2 << endl;
    }
    if(query.exec("SELECT score FROM level3_table where id = " + id))
    {
        query.first();
        score3 = query.value(0).toString().toStdString();
        cout << "user " << sent_user << " level 3 score: " << score3 << endl << endl;
    }

    return "Level1 " + score1 + " Level2 " + score2 + " Level3 " + score3;
}

bool updateScore(string sent_user, string sent_password, string score, string level, QSqlQuery query)
{
    // Query to update a level score
    bool updatedScore = false;
    QVariant var_id;
    if(query.exec("SELECT id FROM User_table WHERE user = '" + QString::fromStdString(sent_user) + "' AND password = '" + QString::fromStdString(sent_password) + "'"))
    {
        query.first();
        var_id = query.value(0);
    }
    //query.prepare("REPLACE INTO Level" + QString::number(level) + "_Table SET id = " + var_id.toString() + ", score = " + QString::number(score)));
    if(query.exec("REPLACE INTO Level" + QString::fromStdString(level) + "_Table SET id = " + var_id.toString() + ", score = " + QString::fromStdString(score)))
    {
        cout << "Inserted score :" << score << " for user: " << sent_user << " into Level" << level << "_Table" << endl << endl;
        if(query.size() > 0)
            updatedScore = true;
    }
    else
    {
        cout << "level update failed!" << endl;
        cout << query.lastQuery().toStdString() << endl;
    }
    return updatedScore;
}
