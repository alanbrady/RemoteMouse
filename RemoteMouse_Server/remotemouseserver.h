#ifndef REMOTEMOUSESERVER_H
#define REMOTEMOUSESERVER_H

#include <QTcpServer>
#include <QList>
#include "remotemouseserverthread.h"
#include "clientidinterface.h"



class RemoteMouseServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit RemoteMouseServer(QObject *parent = 0);
    ~RemoteMouseServer();

signals:
    void createSocket();

public slots:
    void socketThreadMessage(QString str);
    void socketThreadError(QString error);

protected:
    void incomingConnection(qintptr handle);

private:
    QMutex m_idsMutex;
    ClientIdInterface* m_ids;
    QList<RemoteMouseServerThread*> clientThreads;

private slots:
    void threadFinished();

};

#endif // REMOTEMOUSESERVER_H
