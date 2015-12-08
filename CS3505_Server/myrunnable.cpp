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
           std::string allIncomming = str.toStdString();
           while(allIncomming.length() > 0)
           {
               std::string incomming ;
               if(allIncomming.find(';') != -1)
               {
                incomming = allIncomming.substr(0,allIncomming.find_first_of(';'));
                allIncomming = allIncomming.substr(allIncomming.find_first_of(';')+1);
               }
               else
               {
                incomming = allIncomming;
                allIncomming.clear();
               }
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
               socket.write(s.c_str());
               socket.flush();
               socket.waitForBytesWritten();
               command.clear();
           }
    }

    socket.close();
}
