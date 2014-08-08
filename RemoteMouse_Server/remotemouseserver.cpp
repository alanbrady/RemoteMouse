#include "remotemouseserver.h"

RemoteMouseServer::RemoteMouseServer(QObject *parent) :
    QTcpServer(parent)
{
    m_ids = ClientIdInterface::instance();
}

RemoteMouseServer::~RemoteMouseServer()
{
}

void RemoteMouseServer::socketThreadMessage(QString str)
{
    qDebug() << "Rcvd: " << str;
}

void RemoteMouseServer::socketThreadError(QString error)
{

}

void RemoteMouseServer::incomingConnection(qintptr handle)
{
    RemoteMouseServerThread* mouseThread =
            new RemoteMouseServerThread(handle, m_ids, this);
    connect(mouseThread, SIGNAL(finished()), mouseThread, SLOT(deleteLater()));

    connect(mouseThread, SIGNAL(statusMessage(QString)),
            this, SLOT(socketThreadMessage(QString)));

    connect(mouseThread, SIGNAL(serverError(QString)),
            this, SLOT(socketThreadError(QString)));
    mouseThread->start();
}
