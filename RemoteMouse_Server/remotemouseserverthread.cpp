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

void RemoteMouseServerThread::parseReadData(char *data)
{
    QString status = "";
    if (strncmp(data, "CHAL_REQ", 8) == 0) {
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
    } else if (strncmp(data, "MOUS_MOV", 8) == 0) {
        if (m_isVerified)
            parseMouseMoveData(data);
        else {
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
    // -- consider revising this to pixels instead of percents
//    data += 8; // jump past socket tag
//    double xPerc;
//    double yPerc;
//    char xStr[7];
//    char yStr[7];
//    xStr[6] = '0'; // set last bits to null for sscanf
//    yStr[6] = '0';
//    memcpy(xStr, data, 6);
//    data += 6; // jump past x data
//    memcpy(yStr, data, 6);


//    // parse data into a double
//    if (sscanf(xStr, "%lf", &xPerc) != 1) {
//        QString fail("Error: failed to parse x perc");
//        emit serverError(fail);
//    }
//    if (sscanf(yStr, "%lf", &yPerc) != 1) {
//        QString fail("Error: failed to parse y perc");
//        emit serverError(fail);
//    }


//    QDesktopWidget* desktop = QApplication::desktop();
//    QCursor c = desktop->cursor();
//    int xDif = m_screenDims.x()*xPerc;
//    int yDif = m_screenDims.y()*yPerc;
//    QPoint pos = desktop->mapToGlobal(c.pos());
//    pos.setX(pos.x()+xDif);
//    pos.setY(pos.y()+yDif);
//    desktop->setCursor(c);

    // Interpret mouse move data.
    // Input string: MOUS_DAT<x amount><y amount>

    data += 8; // increment data pointer past tag
    char xStr[7];
    char yStr[7];
    int xAmt;
    int yAmt;


    memcpy(xStr, data, 6); // copy x amount to str
    data += 6;
    memcpy(yStr, data, 6); // copy y amount to str

    // parse the x/y strings into ints
    if (sscan(xStr, "%d", &xAmt) != 1) {
        QString fail("Error: failed to parse mouse move x");
        emit serverError(fail);
    }

    if (sscan(yStr, "%d", &yAmt) != 1) {
        QString fail("Error: failed to parse mouse mvoe y");
        emit serverError(fail);
    }

    // adjust cursor
    QDesktopWidget* desktop = QApplication::desktop();
    QCursor c = desktop->cursor();
    QPoint pos = desktop->mapToGlobal(c.pos());
    pos.setX(pos.x()+xAmt);
    pos.setY(pos.y()+yAmt);
    c.setPos(pos);
    desktop->setCursor(c);
}

void RemoteMouseServerThread::performMouseClick()
{
    // Needed utilities outside of Qt API, will need platform specific
    // code here

    // Windows code
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

    // I don't necessarily like the deviation from AAAA_BBB but the alternative
    // would be something like CHAL_PAS/CHAL_FAL which I like less
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
        parseReadData(data);
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

