#ifndef REMOTEMOUSESERVERTHREAD_H
#define REMOTEMOUSESERVERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QMutex>
#include <QDataStream>
#include <QCursor>
#include <windows.h>
#include <QApplication>
#include <QDesktopWidget>
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
    QByteArray m_challenge;

    void parseReadData(char* data, int dataLen);
    void parseMouseMoveData(char* data, int dataLen);
    void performMouseClick(char* data, int dataLen);
    void sendChallenge();
    const QByteArray generateChallenge();
    bool verifyResponse(const char *data);

};

#endif // REMOTEMOUSESERVERTHREAD_H
