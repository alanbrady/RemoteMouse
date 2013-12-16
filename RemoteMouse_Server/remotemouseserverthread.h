#ifndef REMOTEMOUSESERVERTHREAD_H
#define REMOTEMOUSESERVERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QMutex>
#include <QDataStream>
#include "clientidinterface.h"

#define MAX_READ 100
#define CHALLENGE_LEN 50

class RemoteMouseServerThread : public QThread
{
    Q_OBJECT
public:
    explicit RemoteMouseServerThread(quintptr socketDesc, ClientIdInterface* ids,
                                     QObject *parent = 0);
    ~RemoteMouseServerThread();

    void run();

signals:
    void socketError(QTcpSocket::SocketError);
    void serverError(QString);

private:
    int m_socketDesc;
    QTcpSocket* m_socket;
    ClientIdInterface* m_ids;
    bool m_isVerified;
    const char* m_challenge;

    void parseReadData(char* data, int dataLen);
    void sendChallenge();
    const QByteArray generateChallenge();
    bool verifyResponse(const char *data);

};

#endif // REMOTEMOUSESERVERTHREAD_H
