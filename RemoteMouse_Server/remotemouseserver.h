#ifndef REMOTEMOUSESERVER_H
#define REMOTEMOUSESERVER_H

#include <QTcpServer>
#include "remotemouseserverthread.h"

class RemoteMouseServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit RemoteMouseServer(QObject *parent = 0);

signals:


public slots:

protected:
    void incomingConnection(qintptr handle);

private:
    QMutex m_mutex;

};

#endif // REMOTEMOUSESERVER_H
