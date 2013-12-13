#include "remotemouseserver.h"

RemoteMouseServer::RemoteMouseServer(QObject *parent) :
    QTcpServer(parent)
{
}

void RemoteMouseServer::incomingConnection(qintptr handle)
{
    RemoteMouseServerThread* mouseThread =
            new RemoteMouseServerThread(handle, &m_mutex, this);
    connect(mouseThread, SIGNAL(finished()), mouseThread, SLOT(deleteLater()));
    mouseThread->start();
}
