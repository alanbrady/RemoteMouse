#include "remotemouseserverthread.h"

RemoteMouseServerThread::RemoteMouseServerThread(quintptr socketDesc,
                                                 ClientIdInterface *ids,
                                                 QObject *parent) :
    QThread(parent), m_socketDesc(socketDesc), m_ids(ids),
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

const QByteArray RemoteMouseServerThread::generateChallenge()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    for (int i = 0; i < CHALLENGE_LEN; i++) {
        stream << static_cast<char>(qrand() * 255);
    }
    return data;
}

bool RemoteMouseServerThread::verifyResponse(char *data)
{
    return true;
}

