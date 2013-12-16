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
            int dataLen = m_socket->read(data, MAX_READ);
            if (dataLen != -1) {
                parseReadData(data, );
            } else {
                QString fail("Error: failed to read socket data");
                emit serverError(fail);
            }
            delete[] data;
        }
    }
}

void RemoteMouseServerThread::parseReadData(char *data, int dataLen)
{
    if (strncmp(data, "CHAL_REQ", 8) == 0) {
        sendChallenge();
    } else if (strncmp(data, "CHAL_RSP", 8) == 0) {
        if (verifyResponse(data))
            m_isVerified = true;
    } else if (strncmp(data, "MOUS_DAT", 8) == 0) {
        // mouse data should be a +/- percent to move mouse
        // TODO
    } else {
        QString fail("Error: bad client request");
        emit serverError(fail);
    }
}

void RemoteMouseServerThread::sendChallenge()
{
    const QByteArray challenge = generateChallenge();
    if (m_socket->isWritable()) {
        m_socket->write(challenge);
        m_socket->waitForBytesWritten();
    } else {
        QString fail("Error: socket is not writable");
        emit serverError(fail);
    }
}

const QByteArray RemoteMouseServerThread::generateChallenge() const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    for (int i = 0; i < CHALLENGE_LEN; i++) {
        stream << static_cast<char>(qrand() * 255);
    }
    return data;
}

bool RemoteMouseServerThread::verifyResponse(const char *data, int dataLen)
{
    QByteArray hashed;
    QByteArray key = m_ids->getKeyForClient();
    data = data+8; // go past socket tag

    return true;
}

