#include "remotemouseserverthread.h"

RemoteMouseServerThread::RemoteMouseServerThread(quintptr socketDesc,
                                                 ClientIdInterface *ids,
                                                 QObject *parent) :
    QThread(parent), m_socketDesc(socketDesc), m_ids(ids),
    m_isVerified(false)
{
    screenDims = QApplication::desktop()->screenGeometry();
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
                parseReadData(data);
            } else {
                QString fail("Error: failed to read socket data");
                emit serverError(fail);
            }
            delete[] data;
        }
    }
}

void RemoteMouseServerThread::parseReadData(char *data)
{
    if (strncmp(data, "CHAL_REQ", 8) == 0) {
        sendChallenge();
    } else if (strncmp(data, "CHAL_RSP", 8) == 0) {
        m_isVerified = verifyResponse(data);
    } else if (strncmp(data, "MOUS_MOV", 8) == 0) {
        if (m_isVerified)
            parseMouseMoveData(data);
        else {
            m_socket->close();
            QString fail("Error: attempt to send mouse data without verification");
            emit serverError(fail);
        }
    } else if (strncmp(data, "MOUS_CLK", 8) == 0) {
        if (m_isVerified)
            performMouseClick();
        else {
            m_socket->close();
            QString fail("Error: attempt to send mouse click without verification");
            emit serverError(fail);
        }
    } else {
        QString fail("Error: bad client request");
        emit serverError(fail);
    }
}

void RemoteMouseServerThread::parseMouseMoveData(char *data)
{
    // Interpret mouse move data
    // mouse data should be a +/- percent to move mouse
    // Format should be MOUS_DAT(+/-)0.---(+/-)0.---
    // First number is x percent, second number is y percent
    data += 8; // jump past socket tag
    double xPerc;
    double yPerc;
    char xStr[7];
    char yStr[7];
    *(xStr+6) = '0'; // set last bits to null for sscanf
    *(yStr+6) = '0';
    memcpy(xStr, data, 6);
    data += 6; // jump past x data
    memcpy(yStr, data, 6);


    // parse data into a double
    if (sscanf(xStr, "%lf", &xPerc) != 1) {
        QString fail("Error: failed to parse x perc");
        emit serverError(fail);
    }
    if (sscanf(yStr, "%lf", &yPerc) != 1) {
        QString fail("Error: failed to parse y perc");
        emit serverError(fail);
    }


    QDesktopWidget* desktop = QApplication::desktop();
    QCursor c = desktop->cursor();
    int xDif = screenDims.x()*xPerc;
    int yDif = screenDims.y()*yPerc;
    QPoint pos = desktop->mapToGlobal(c.pos());
    pos.setX(pos.x()+xDif);
    pos.setY(pos.y()+yDif);
    desktop->setCursor(c);
}

void RemoteMouseServerThread::performMouseClick()
{
    // Needed utilities outside of Qt API, will need platform specific
    // code here
    mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP,
                1, 1, 0, 0);
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

const QByteArray RemoteMouseServerThread::generateChallenge()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    for (int i = 0; i < CHALLENGE_LEN; i++) {
        stream << static_cast<char>(qrand() * 255);
    }
    m_challenge = data;
    return data;
}

bool RemoteMouseServerThread::verifyResponse(const char *data)
{
    char* hashed;
    int idLen = m_ids->getIdLen();
    data += 8; // go past socket tag
    QString id = QString::fromLocal8Bit(data, idLen);
    data += idLen; // go past id
    const QByteArray keyByteArray = m_ids->getKeyForClient(id);
    const char* key = keyByteArray.constData();
    const char* keyStart = key;
    int keyLen = keyByteArray.length();

    hashed = new char[keyLen];
    char* hashStart = hashed;
    char* hashEnd = hashed+8;
    while (hashed != hashEnd) *(hashed++) = '0';
    hashed = hashStart;

    char* response = new char[keyLen];
    strncpy(response, data, keyLen);

    const char* challenge = m_challenge.constData();
    const char* challengeEnd = challenge + CHALLENGE_LEN;
    while (challenge != challengeEnd) {
        if (hashed == hashEnd) {
            hashed = hashStart;
            key = keyStart;
        }
        *(hashed) = (*challenge++) ^ (*key++) ^ (*hashed);
        hashed++;
    }
    bool isEqual = false;
    if (strncmp(response, hashed, keyLen) == 0)
        isEqual = true;

    delete[] hashed;
    return isEqual;
}

