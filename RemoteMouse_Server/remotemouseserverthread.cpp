#include "remotemouseserverthread.h"

RemoteMouseServerThread::RemoteMouseServerThread(quintptr socketDesc,
                                                 ClientIdInterface *ids,
                                                 QObject *parent) :
    QThread(parent), m_socketDesc(socketDesc), m_socket(0), m_ids(ids),
    m_isVerified(false)
{
    m_screenDims = QApplication::desktop()->screenGeometry();

}

RemoteMouseServerThread::~RemoteMouseServerThread()
{
    if (m_socket != 0) {
        if (m_socket->isOpen()) {
            m_socket->close();
        }
        delete m_socket;
    }

}

void RemoteMouseServerThread::run()
{
    connect(this, SIGNAL(createSocketSignal()),
            this, SLOT(createSocket()), Qt::QueuedConnection);
    emit createSocketSignal();
    exec();
}

void RemoteMouseServerThread::parseReadData()
{
    while (!m_socketDataQueue.isEmpty()) {
        SocketDataQueue::SocketData socketData = m_socketDataQueue.dequeueData();
        const char* data = socketData.getData();
        QString status = "";
        if (strncmp(data, "MOUS_DAT", 8) == 0) {
            if (m_isVerified) {
                parseMouseMoveData(data);

            } else {
                // close socket and emit failure message if attenmpts to send mouse data
                // without passing verification
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
        } else if (strncmp(data, "CHAL_REQ", 8) == 0) {
            status = "Challenge requested from: ";
            status += m_peerAddress;
            emit statusMessage(status);
            sendChallenge();
        } else if (strncmp(data, "CHAL_RSP", 8) == 0) {
            status = "Challenge response received from: ";
            status += m_peerAddress;
            emit statusMessage(status);
            m_isVerified = verifyResponse(data);
            sendVerificationStatus();
            if (m_isVerified) {
                status = "Client is verified: ";
            } else {
                status = "Client failed verification: ";
                if (m_socket->bytesToWrite() > 0) {
                    m_socket->waitForBytesWritten();
                }
                m_socket->close();
            }
            status += m_peerAddress;
            emit statusMessage(status);
        } else {
            QString fail("Error: bad client request - ");
            fail += data;
            emit serverError(fail);
        }
    }
}

void RemoteMouseServerThread::parseMouseMoveData(const char *data)
{
    // Interpret mouse move data.
    // Input string: MOUS_DAT<x amount><y amount>

    data += 8; // increment data pointer past tag
    char xStr[8];
    char yStr[8];
    double xAmt;
    double yAmt;


    memcpy(xStr, data, 8); // copy x amount to str
    data += 8;
    memcpy(yStr, data, 8); // copy y amount to str

    // change endianess
    byteSwap8(xStr);
    byteSwap8(yStr);
//    xAmt = *((double*)xStr);
//    yAmt = *((double*)yStr);
    memcpy(&xAmt, xStr, 8);
    memcpy(&yAmt, yStr, 8);

//    qDebug() << "Mouse move: " << xAmt << " " << yAmt;

    // adjust cursor
    QDesktopWidget* desktop = QApplication::desktop();
    QCursor c = desktop->cursor();
    QPoint pos = desktop->mapToGlobal(c.pos());
    double iter = 500;
    double gradX = xAmt/iter;
    double gradY = yAmt/iter;
    double posX = pos.x();
    double posY = pos.y();
    for (int i = 0; i < iter; i++) {
        posX += gradX;
        posY += gradY;
        pos.setX(posX);
        pos.setY(posY);
        c.setPos(pos);
    }
//    pos.setX(pos.x()+xAmt);
//    pos.setY(pos.y()+yAmt);
//    c.setPos(pos);
//    desktop->setCursor(c);
}

void RemoteMouseServerThread::byteSwap8(void *v)
{
    char in[8], out[8];
    memcpy(in, v, 8);
    for (int i = 0; i < 8; i++) {
        out[i] = in[7-i];
    }
    memcpy(v, out, 8);
}

void RemoteMouseServerThread::performMouseClick()
{
    // Needed utilities outside of Qt API, will need platform specific
    // code here

    // Windows code
//    qDebug() << "Mouse click received";
    mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP,
                0, 0, 0, 0);

    // TODO - other platforms if needed
}

void RemoteMouseServerThread::sendChallenge()
{
//    const QByteArray challenge = generateChallenge();
    generateChallenge();
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write(m_challenge, CHALLENGE_LEN);
        m_socket->write("\n");
        m_socket->flush();
        m_socket->waitForBytesWritten();
    } else {
        QString fail("Error: socket is not connected!");
        emit serverError(fail);
    }
}

// randomly generate a sequence of characters
// IDEA: random challenge length, right now it's static but I not sure if that
//       makes a huge difference
// Challenge cannot include '\n' because that is the terminating character for
// the client to determine the end of the challenge string.  A better
// implementation would be to transmit the length of the challenge string
// to the client.  This would also more aptly allow a variable length challenge
// string.
void RemoteMouseServerThread::generateChallenge()
{
    srand(time(NULL));
    for (int i = 0; i < CHALLENGE_LEN; i++) {
        int k = 0;
        do {
            k = rand() % 256;
        } while (k == '\n');
        m_challenge[i] = k;
    }
}

// response data is in the form {(id)(repsonse)}
bool RemoteMouseServerThread::verifyResponse(const char *data)
{
    char* hashed;
    data += 8; // go past socket tag
    int idLen = char(*(data));
    data++;
    QString id = QString::fromLocal8Bit(data, idLen);
    data += idLen; // go past id
    const QByteArray keyByteArray = m_ids->getKeyForClient(id).toLocal8Bit();
    const char* key = keyByteArray.constData();
    const char* keyStart = key;
    int keyLen = keyByteArray.length();

    hashed = new char[keyLen];
    char* hashStart = hashed;
    char* hashEnd = hashed+keyLen;
    memset(hashed, 0, keyLen*sizeof(char));

    char* response = new char[keyLen];
    memcpy(response, data, keyLen);

    const char* challenge = m_challenge;
    const char* challengeEnd = challenge + CHALLENGE_LEN;
    while (challenge != challengeEnd) {
        if (hashed == hashEnd) {
            hashed = hashStart;
            key = keyStart;
        }
        (*hashed) = ((*challenge) ^ (*key)) ^ (*hashed);
        key++; challenge++; hashed++;
    }
    hashed = hashStart;
    bool isEqual = (memcmp(response,hashed,keyLen) == 0);

    delete[] hashed;
    return isEqual;
}

void RemoteMouseServerThread::sendVerificationStatus()
{
    QString valStatus = "";
    QString status = "Sending verification status (";

    if (m_isVerified)  {
        valStatus = "CHALPASS";
        status += "Pass";
    } else {
        valStatus = "CHALFAIL";
        status += "Fail";
    }

    status += "): ";
    status += m_peerAddress;
    emit statusMessage(status);

    m_socket->write(valStatus.toStdString().c_str());
    m_socket->flush();
    m_socket->waitForBytesWritten();
}

void RemoteMouseServerThread::socketReadyRead()
{
    qint64 bytes = m_socket->bytesAvailable();
    char* data = new char[bytes];

    int dataLen = 0;
    if (m_socket->isOpen()) {
        dataLen = m_socket->read(data, bytes);
    }
    if (dataLen == -1) {
        QString fail("Error: failed to read socket data");
        emit serverError(fail);
    } else {
        m_socketDataQueue.addData(data, dataLen);
        parseReadData();
    }

    delete[] data;
}

void RemoteMouseServerThread::createSocket()
{
    m_socket = new QTcpSocket(this);
    if (!m_socket->setSocketDescriptor(m_socketDesc)) {
        emit socketError(m_socket->error());
    } else {
        connect(m_socket, SIGNAL(readyRead()),
                this, SLOT(socketReadyRead()));
        connect(m_socket, SIGNAL(disconnected()),
                this, SLOT(socketClosed()));
        m_peerAddress = m_socket->peerAddress().toString();
        QString status = "Socket initialized: ";
        status += m_peerAddress;
        emit statusMessage(status);
    }
}

void RemoteMouseServerThread::socketClosed()
{
    QString status = "Socket closed: ";
    status += m_peerAddress;
    emit statusMessage(status);
    quit();
}

