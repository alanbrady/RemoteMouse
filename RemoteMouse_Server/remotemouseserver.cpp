#include "remotemouseserver.h"

RemoteMouseServer::RemoteMouseServer(QObject *parent) :
    QTcpServer(parent)
{
    m_ids = ClientIdInterface::instance();
}

RemoteMouseServer::~RemoteMouseServer()
{
}

void RemoteMouseServer::incomingConnection(qintptr handle)
{
    RemoteMouseServerThread* mouseThread =
            new RemoteMouseServerThread(handle, m_ids, this);
    connect(mouseThread, SIGNAL(finished()), mouseThread, SLOT(deleteLater()));
    mouseThread->start();
}
