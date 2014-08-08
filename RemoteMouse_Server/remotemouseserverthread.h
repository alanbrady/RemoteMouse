#ifndef REMOTEMOUSESERVERTHREAD_H
#define REMOTEMOUSESERVERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
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
    void statusMessage(QString);

private:
    int m_socketDesc;
    QTcpSocket* m_socket;
    ClientIdInterface* m_ids;
    bool m_isVerified;
    QByteArray m_challenge;
    QRect m_screenDims;
    QString m_peerAddress;

    void parseReadData(char* data);
    void parseMouseMoveData(char* data);
    void performMouseClick();
    void sendChallenge();
    const QByteArray generateChallenge();
    bool verifyResponse(const char *data);

};

#endif // REMOTEMOUSESERVERTHREAD_H
