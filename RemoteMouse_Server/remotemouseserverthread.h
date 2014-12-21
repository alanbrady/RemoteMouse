#ifndef REMOTEMOUSESERVERTHREAD_H
#define REMOTEMOUSESERVERTHREAD_H

#include <stdlib.h>
#include <time.h>

#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMutex>
#include <QDataStream>
#include <QCursor>
#include <QApplication>
#include <QDesktopWidget>
#include <windows.h>

#include "clientidinterface.h"
#include "socketdataqueue.h"

#define MAX_READ 100
#define CHALLENGE_LEN 50

class RemoteMouseServerThread : public QThread
{
    Q_OBJECT
public:
    explicit RemoteMouseServerThread(quintptr socketDesc, ClientIdInterface* ids,
                                     QObject *parent = 0);
    ~RemoteMouseServerThread();

    enum Message {
        MOUSE_DATA = 1,
        MOUSE_CLICK,
        CHALLENGE_REQUEST,
        CHALLENGE_RESPONSE
    };

    void run();

    qintptr socketDescriptor() const { return m_socketDesc; }

signals:
    void socketError(QTcpSocket::SocketError);
    void serverError(QString);
    void statusMessage(QString);
    void createSocketSignal();

private:
    int m_socketDesc;
    QTcpSocket* m_socket;
    ClientIdInterface* m_ids;
    bool m_isVerified;
    char m_challenge[CHALLENGE_LEN];
    QRect m_screenDims;
    QString m_peerAddress;
    SocketDataQueue m_socketDataQueue;

    void parseReadData(const char* data);
    void parseMouseMoveData(const char *data);
    void byteSwap8(void* v);
    void performMouseClick();
    void sendChallenge();
    void generateChallenge();
    bool verifyResponse(const char *data);
    void sendVerificationStatus();


private slots:
    void socketReadyRead();
    void createSocket();
    void socketClosed();

};

#endif // REMOTEMOUSESERVERTHREAD_H
