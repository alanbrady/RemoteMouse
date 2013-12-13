#include "remotemouseserverthread.h"

RemoteMouseServerThread::RemoteMouseServerThread(quintptr socketDesc, QMutex *mutex,
                                                 QObject *parent) :
    QThread(parent), m_socketDesc(socketDesc), m_sharedMutex(mutex),
    m_isVerified(false)
{
}

RemoteMouseServerThread::~RemoteMouseServerThread()
{
    if (m_socket != 0)
        delete m_socket;

}

void RemoteMouseServerThread::run()
{
    m_socket = new QTcpSocket();

    if (!m_socket->setSocketDescriptor(m_socketDesc)) {
        emit socketError(m_socket->error());
    } else {
        while (m_socket->isOpen()) {
            m_socket->waitForReadyRead();
            char* data = new char[MAX_READ];
            m_socket->read(data, MAX_READ);
        }
    }
}

void RemoteMouseServerThread::parseReadData(char *data)
{
    if (strncmp(data, "CHAL_REQ", 8) == 0) {
        sendChallenge();
    } else if (strncmp(data, "CHAL_RSP", 8) == 0) {
        if (verifyResponse(data))
            m_isVerified = true;
    } else if (strncmp(data, "MOUS_DAT", 8) == 0) {
        // TODO
    } else {
        QString fail("Error: bad client request");
        emit serverError(fail);
    }
}

void RemoteMouseServerThread::sendChallenge()
{
    // TODO
}

bool RemoteMouseServerThread::verifyResponse(char *data)
{
    // TODO
    return true;
}

