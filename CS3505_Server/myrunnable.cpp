// MyRunnalbe.cpp

#include "myrunnable.h"

MyRunnable::MyRunnable()
{
}

void MyRunnable::run()
{
    if(!socketDescriptor) return;

    QTcpSocket socket;
    socket.setSocketDescriptor(socketDescriptor);

    socket.write("From server: hello world");
    socket.flush();
    socket.waitForBytesWritten();
    socket.close();
}
