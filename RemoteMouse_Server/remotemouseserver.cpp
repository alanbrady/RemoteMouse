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
        delete thread;
    }
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

    clientThreads.append(mouseThread);

    connect(mouseThread, SIGNAL(finished()), mouseThread, SLOT(deleteLater()));
    connect(mouseThread, SIGNAL(statusMessage(QString)),
            this, SLOT(socketThreadMessage(QString)));
    connect(mouseThread, SIGNAL(serverError(QString)),
            this, SLOT(socketThreadError(QString)));
    connect(mouseThread, SIGNAL(finished()), this, SLOT(threadFinished()));
    connect(this, SIGNAL(createSocket()),
            mouseThread, SLOT(createSocket()));

    mouseThread->start();
    emit(createSocket());
}

void RemoteMouseServer::threadFinished()
{
    RemoteMouseServerThread* thread = dynamic_cast<RemoteMouseServerThread*>(sender());
    if (thread != 0) {
        clientThreads.removeAll(thread);
        delete thread;
    }
}
