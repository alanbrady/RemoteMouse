#ifndef REMOTEMOUSESERVERTHREAD_H
#define REMOTEMOUSESERVERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QMutex>
#include <QDataStream>

#define MAX_READ 100

class RemoteMouseServerThread : public QThread
{
    Q_OBJECT
public:
    explicit RemoteMouseServerThread(quintptr socketDesc, QMutex* mutex,
                                     QObject *parent = 0);
    ~RemoteMouseServerThread();

    void run();

signals:
    void socketError(QTcpSocket::SocketError);
    void serverError(QString);

private:
    int m_socketDesc;
    QTcpSocket* m_socket;
    QMutex *m_sharedMutex;
    bool m_isVerified;

    void parseReadData(char* data);
    void sendChallenge();
    bool verifyResponse(char* data);

};

#endif // REMOTEMOUSESERVERTHREAD_H
