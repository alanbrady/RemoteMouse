#ifndef REMOTEMOUSESERVER_H
#define REMOTEMOUSESERVER_H

#include <QTcpServer>
#include "remotemouseserverthread.h"
#include "clientidinterface.h"

class RemoteMouseServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit RemoteMouseServer(QObject *parent = 0);
    ~RemoteMouseServer();

signals:


public slots:

protected:
    void incomingConnection(qintptr handle);

private:
    QMutex m_idsMutex;
    ClientIdInterface* m_ids;

};

#endif // REMOTEMOUSESERVER_H
