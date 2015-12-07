// MyRunnalbe.cpp

#include "myrunnable.h"
#include "iostream"

MyRunnable::MyRunnable(DataBase *db)
{
    this->db = db;
    mutex = new QMutex;
}

void MyRunnable::run()
{
    if(!socketDescriptor) return;

    QTcpSocket socket;
    socket.setSocketDescriptor(socketDescriptor);

    socket.write("From server: hello world");
    socket.flush();
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
//    qDebug() << " Reading: " << socket.bytesAvailable();
//    qDebug() << socket.readAll();
    QList<std::string> command;
    //May need to create (monitor, mutex, semephore) for this
    if(socket.bytesAvailable() > 0)
    {
           QByteArray qba = socket.readAll();
           QString str(qba);
           std::string incomming = str.toStdString();

           while(incomming.length() > 0)
           {
               if(incomming.find(' ') != -1)
               {
                   command.push_back(incomming.substr(0,incomming.find_first_of(' ')));
                   incomming = incomming.substr(incomming.find_first_of(' ')+1);
               }
               else
               {
                   command.push_back(incomming);
                   incomming.clear();
               }
           }
           std::string s = db->runCommand(command);
           std::cout << s;
    }

    socket.close();
}
