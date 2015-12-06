// myrunnable.h

#ifndef MYRUNNABLE_H
#define MYRUNNABLE_H

#include <QRunnable>
#include <QTcpSocket>
#include <QDebug>
#include <string>
#include <QList>
#include "database.h"
#include <QMutexLocker>


class MyRunnable : public QRunnable
{
public:
    MyRunnable(DataBase *db);

protected:
    void run();

public:
    qintptr socketDescriptor;
    DataBase *db;
    QMutex *mutex;
};

#endif // MYRUNNABLE_H
