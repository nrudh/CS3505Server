#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QtSql>

class DataBase
{
public:
    QSqlDatabase* db;
    QSqlQuery* query;
    QMutex *mutex;
    DataBase();
    std::string runCommand(QList<std::string> command);
    std::string dispAllUsers();
    bool addUser(std::string sent_user, std::string sent_password);
    bool userExists(std::string sent_user, std::string sent_password);
    bool deleteUser(std::string sent_user, std::string sent_password);
    std::string getUserState(std::string sent_user, std::string sent_password);
    bool updateScore(std::string sent_user, std::string sent_password, std::string score, std::string level);
};

#endif // DATABASE_H