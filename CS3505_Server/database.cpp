#include "database.h"
#include <iostream>

DataBase::DataBase()
{
    db =  new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));
    db->setHostName("73.3.77.26");
    //db.setHostName("192.168.1.118");
    db->setPort(3506);
    db->setDatabaseName("thegeographers");
    db->setUserName("student");
    db->setPassword("CS3505");
    query = new QSqlQuery(*db);
    mutex = new QMutex;
}

std::string DataBase::runCommand(QList<std::string> command)
{
    QMutexLocker locker(mutex);
    if(command[0] == "login")
    {
        if(command.size() != 3)
            return "Invalid command";
        if(userExists(command[1], command[2]))
            return "true";
        return "false";
    }
    if(command[0] == "scores")
    {
        if(command.size() != 3)
            return "Invalid command";
        return getUserState(command[1], command[2]);
    }
    if(command[0] == "adduser")
    {
        if(command.size() != 3)
            return "Invalid command";
        if(addUser(command[1], command[2]))
            return "true";
        return "false";
    }
    if(command[0] == "update")
    {
        if(command.size() != 5)
            return "Invalid command";
        if(updateScore(command[1], command[2], command[3], command[4]))
            return "true";
        return "false";
    }
    if(command[0] == "delete")
    {
        if(command.size() != 3)
            return "Invalid command";
        if(deleteUser(command[1], command[2]))
            return "true";
        return "false";
    }
    if(command[0] == "display")
    {
        if(command.size() != 1)
            return "false";
        else
            return dispAllUsers();
    }
}

std::string DataBase::dispAllUsers()
{
    std::vector<std::string> usersVect;
    std::string str;
    if(db->open())
    {
        // Select * query
        query->prepare("SELECT * from User_table");
        if(query->exec())
        {
            std::string buildStr;
           while(query->next())
           {
                   QString id = query->value(0).toString();
                   QString user = query->value(1).toString();
                   QString password = query->value(2).toString();
                   buildStr.push_back("user: " + user.toStdString() + " passwrod: " + password.toStdString() + " " + getUserState(user.toStdString(), password.toStdString(), 1) + "\n");
                   str += id.toStdString() + " " + user.toStdString() + " " + password.toStdString() + "\n";
           }
        }
        else
        {
            db->close();
             return "Query failed!";
        }
        db->close();
    }
    //return str;
    return buildStr;
}

bool DataBase::addUser(std::string sent_user, std::string sent_password)
{
    bool userAdded = false;
    if(db->open())
    {
        if(userExists(sent_user, sent_password) == false)
        {
            if(db->open())
            {
                query->prepare("INSERT INTO User_table (user, password) "
                              "VALUES (:user, :password)");
                query->bindValue(":user", QString::fromStdString(sent_user));
                query->bindValue(":password", QString::fromStdString(sent_password));
                if(query->exec())
                {
                    qDebug() << query->executedQuery();
                    if(query->numRowsAffected() > 0)
                         userAdded = true;
                }
            }
        }
    }
    db->close();
    return userAdded;
}

bool DataBase::userExists(std::string sent_user, std::string sent_password)
{
    if(db->open())
    {
        // Query if user and password exists
        //query->prepare("SELECT id FROM User_table WHERE user = 'Nick' AND password = 56789");
        //query->bindValue(":user",QString::fromStdString(sent_user));
        //query->bindValue(":password",QString::fromStdString(sent_password));
        //query->prepare("SELECT id FROM User_table WHERE user = '" + QString::fromStdString(sent_user) + "' AND password = '" + QString::fromStdString(sent_password) + "'");
        if(query->exec("SELECT id FROM User_table WHERE user = '" + QString::fromStdString(sent_user) + "' AND password = '" + QString::fromStdString(sent_password) + "'"))
        {
            qDebug() << query->executedQuery();
            if(query->numRowsAffected() > 0)
            {
                query->first();
                db->close();
                return true;
            }
            else
                qDebug() << query->lastError().text();
        }
        db->close();
        return false;
    }
}

bool DataBase::deleteUser(std::string sent_user, std::string sent_password)
{
    if(db->open())
    {
        // Query to delete a user
        bool deleted = false;
        query->exec("SET FOREIGN_KEY_CHECKS = 0;");
        if(query->exec("DELETE FROM User_table WHERE user = '" + QString::fromStdString(sent_user) + "' AND password = '" + QString::fromStdString(sent_password) + "'"))
        {
            if(query->numRowsAffected() > 0)
                deleted = true;
        }
        db->close();
        return deleted;
    }
}

std::string DataBase::getUserState(std::string sent_user, std::string sent_password)
{
    if(db->open())
    {
        // Query to get the state of a user
        std::string score1, score2, score3;
        if(query->exec("SELECT l1.score FROM User_table us, level1_table l1 WHERE us.id = l1.id AND us.user = '" + QString::fromStdString(sent_user) +  "'" + " AND us.password = '" + QString::fromStdString(sent_password) + "'"))
        {
            //if(query->numRowsAffected() > 0){
                query->first();
                score1 = query->value(0).toString().toStdString();
            //}
        }
        if(query->exec("SELECT l2.score FROM User_table us, level2_table l2 WHERE us.id = l2.id AND us.user = '" + QString::fromStdString(sent_user) +  "'" + " AND us.password = '" + QString::fromStdString(sent_password) + "'"))
        {
            if(query->numRowsAffected() > 0){
                query->first();
                score2 = query->value(0).toString().toStdString();
            }
        }
        if(query->exec("SELECT l3.score FROM User_table us, level3_table l3 WHERE us.id = l3.id AND us.user = '" + QString::fromStdString(sent_user) +  "'" + " AND us.password = '" + QString::fromStdString(sent_password) + "'"))
        {
            if(query->numRowsAffected() > 0){
                query->first();
                score3 = query->value(0).toString().toStdString();
            }
        }

        db->close();
        return "Level1 " + score1 + " Level2 " + score2 + " Level3 " + score3;
    }
}

bool DataBase::updateScore(std::string sent_user, std::string sent_password, std::string level, std::string score)
{
    if(db->open())
    {
        // Query to update a level score
        bool updatedScore = false;
        if(query->exec("REPLACE INTO Level" + QString::fromStdString(level) + "_Table SET id = (SELECT id From User_Table Where user = '" + QString::fromStdString(sent_user) + "' AND password = '" + QString::fromStdString(sent_password) + "'), score = " + QString::fromStdString(score)))
        {
            if(query->numRowsAffected() > 0)
                updatedScore = true;
        }
        db->close();
        return updatedScore;
    }
}

std::string DataBase::getUserState(std::string sent_user, std::string sent_password, int i)
{
    {
        // Query to get the state of a user
        std::string score1, score2, score3;
        if(query->exec("SELECT l1.score FROM User_table us, level1_table l1 WHERE us.id = l1.id AND us.user = '" + QString::fromStdString(sent_user) +  "'" + " AND us.password = '" + QString::fromStdString(sent_password) + "'"))
        {
            //if(query->numRowsAffected() > 0){
                query->first();
                score1 = query->value(0).toString().toStdString();
            //}
        }
        if(query->exec("SELECT l2.score FROM User_table us, level2_table l2 WHERE us.id = l2.id AND us.user = '" + QString::fromStdString(sent_user) +  "'" + " AND us.password = '" + QString::fromStdString(sent_password) + "'"))
        {
            if(query->numRowsAffected() > 0){
                query->first();
                score2 = query->value(0).toString().toStdString();
            }
        }
        if(query->exec("SELECT l3.score FROM User_table us, level3_table l3 WHERE us.id = l3.id AND us.user = '" + QString::fromStdString(sent_user) +  "'" + " AND us.password = '" + QString::fromStdString(sent_password) + "'"))
        {
            if(query->numRowsAffected() > 0){
                query->first();
                score3 = query->value(0).toString().toStdString();
            }
        }

        return "Level1 " + score1 + " Level2 " + score2 + " Level3 " + score3;
    }

