#include "remotemouseserver.h"

RemoteMouseServer::RemoteMouseServer(QObject *parent) :
    QTcpServer(parent)
{
    m_ids = ClientIdInterface::instance();
}

RemoteMouseServer::~RemoteMouseServer()
{
    for (int i = 0; i < clientThreads.size(); i++) {
        RemoteMouseServerThread* thread = clientThreads.at(i);
        thread->quit();
        thread->wait();
    }
}

void RemoteMouseServer::socketThreadMessage(QString str)
{
    qDebug() << str;
}

void RemoteMouseServer::socketThreadError(QString error)
{
    qDebug() << "Error: " << error;
}

void RemoteMouseServer::incomingConnection(qintptr handle)
{
        RemoteMouseServerThread* mouseThread =
                new RemoteMouseServerThread(handle, m_ids, this);

        clientThreads.append(mouseThread);

        connect(mouseThread, SIGNAL(finished()), mouseThread, SLOT(deleteLater()));
        connect(mouseThread, SIGNAL(finished()), this, SLOT(threadFinished()));
        connect(mouseThread, SIGNAL(statusMessage(QString)),
                this, SLOT(socketThreadMessage(QString)));
        connect(mouseThread, SIGNAL(serverError(QString)),
                this, SLOT(socketThreadError(QString)));

        mouseThread->start();
}

void RemoteMouseServer::threadFinished()
{
    RemoteMouseServerThread* thread = dynamic_cast<RemoteMouseServerThread*>(sender());
    if (thread != 0) {
        clientThreads.removeAll(thread);
    }
}
