#include "database.h"

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
}

std::string DataBase::dispAllUsers()
{
    std::string str;
    if(db->open())
    {
        // Select * query
        if(query->exec("SELECT * from User_table"))
        {
           //cout << "Contents of 'User_table' table" << endl;

           while(query->next())
           {
                   QString id = query->value(0).toString();
                   QString user = query->value(1).toString();
                   QString password = query->value(2).toString();
                   //qDebug() << id << " " << username << " " << password << endl;
                   str += id.toStdString() + " " + user.toStdString() + " " + password.toStdString() + "\r\n";
           }
        }
        else
        {
            db->close();
             return "Query failed!";
        }
        db->close();
    }
    return str;
}

bool DataBase::addUser(std::string sent_user, std::string sent_password)
{
    if(db->open())
    {
        bool userAdded = false;
        query->prepare("INSERT INTO User_table (user, password) "
                      "VALUES (:user, :password)");
        query->bindValue(":user", QString::fromStdString(sent_user));
        query->bindValue(":password", QString::fromStdString(sent_password));
        if(query->exec())
        {
            //cout << endl << "Inserted user: " << sent_user << " " << query->numRowsAffected() << endl << endl;
            if(query->size() > 0)
                 userAdded = true;
        }
        else
            //cout << "Insert failed!" << endl;
        db->close();
        return userAdded;
    }
}

bool DataBase::userExists(std::string sent_user, std::string sent_password)
{
    if(db->open())
    {
        // Query if user and password exists
        query->prepare("SELECT * FROM User_table WHERE user = '" + QString::fromStdString(sent_user) + "' AND password = '" + QString::fromStdString(sent_password) + "'");
        if(query->exec())
        {
            //cout << "Found user: " << sent_user << " " <<query->size() << endl << endl;
            if(query->size() > 0)
                return true;
        }
        else
            //cout << "Query failed!" << endl;
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
        if(query->exec("DELETE FROM User_table WHERE user = '" + QString::fromStdString(sent_user) + "' AND password = '" + QString::fromStdString(sent_password) + "'"))
        {
            //cout << "Deleted user: " << sent_user << " " << query->numRowsAffected() << endl << endl;
            if(query->size() > 0)
                deleted = true;
        }
        //else
            //cout << "Delete failed!" << endl;
        db->close();
        return deleted;
    }
}

std::string DataBase::getUserState(std::string sent_user, std::string sent_password)
{
    if(db->open())
    {
        // Query to get the state of a user
        QString id;
        std::string score1, score2, score3;
        if(query->exec("SELECT id FROM User_table WHERE user = '" + QString::fromStdString(sent_user) + "' AND password = '" + QString::fromStdString(sent_password) + "'"))
        {
            query->first();
            id = query->value(0).toString();
        }
        if(query->exec("SELECT score FROM level1_table where id = " + id))
        {
            query->first();
            score1 = query->value(0).toString().toStdString();
            //cout << "user " << sent_user << " level 1 score: " << score1 << endl;
        }
        if(query->exec("SELECT score FROM level2_table where id = " + id))
        {
            query->first();
            score2 = query->value(0).toString().toStdString();
            //cout << "user " << sent_user << " level 2 score: " << score2 << endl;
        }
        if(query->exec("SELECT score FROM level3_table where id = " + id))
        {
            query->first();
            score3 = query->value(0).toString().toStdString();
            //cout << "user " << sent_user << " level 3 score: " << score3 << endl << endl;
        }

        db->close();
        return "Level1 " + score1 + " Level2 " + score2 + " Level3 " + score3;
    }
}

bool DataBase::updateScore(std::string sent_user, std::string sent_password, std::string score, std::string level)
{
    if(db->open())
    {
        // Query to update a level score
        bool updatedScore = false;
        QVariant var_id;
        if(query->exec("SELECT id FROM User_table WHERE user = '" + QString::fromStdString(sent_user) + "' AND password = '" + QString::fromStdString(sent_password) + "'"))
        {
            query->first();
            var_id = query->value(0);
        }
        //query->prepare("REPLACE INTO Level" + QString::number(level) + "_Table SET id = " + var_id.toString() + ", score = " + QString::number(score)));
        if(query->exec("REPLACE INTO Level" + QString::fromStdString(level) + "_Table SET id = " + var_id.toString() + ", score = " + QString::fromStdString(score)))
        {
            //cout << "Inserted score :" << score << " for user: " << sent_user << " into Level" << level << "_Table" << endl << endl;
            if(query->size() > 0)
                updatedScore = true;
        }
        else
        {
            //cout << "level update failed!" << endl;
            //cout << query->lastQuery().toStdString() << endl;
        }
        db->close();
        return updatedScore;
    }
}

