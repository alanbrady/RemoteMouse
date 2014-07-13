#include "clientidinterface.h"

ClientIdInterface* ClientIdInterface::m_instance = new ClientIdInterface();

ClientIdInterface::ClientIdInterface()
{
    m_keyChars = "abcdefghijklmopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\
            1234567890@!?$";
    // calculate length of m_keyChars based on sizeof of char which
    // should actually always be one byte, but it doesn't hurt to double
	// check	
    m_keyCharLen = sizeof(m_keyChars)/sizeof(m_keyChars[0]);
    m_file.setFileName("client_ids.dat");
    srand(time(NULL));
    parseFile();
}

ClientIdInterface::~ClientIdInterface()
{
}

const QString ClientIdInterface::getKeyForClient(const QString &clientId)
{
    QMutexLocker locker(&m_mutex);
    QString key = m_keys.value(clientId, "Error: ID does not exist!");
    return key;
}

const QString ClientIdInterface::generateNewKey() const
{
    QString newKey = "";
    for (int i = 0; i < KEY_LEN; i++) {
        newKey += m_keyChars[rand()%m_keyCharLen];
    }
    return newKey;
}

void ClientIdInterface::setKeyForClient(const QString &clientId,
                                        const QString &clientKey)
{
    m_keys.insert(clientId, clientKey);
    saveKeyToFile(clientId, clientKey);
}

bool ClientIdInterface::removeClient(const QString &clientId)
{
    QMutexLocker locker(&m_mutex);
    bool clientFound = false;
    m_file.open(QFile::ReadOnly | QFile::Text);
    QString data;
    if (m_file.isReadable()) {
        while (!m_file.atEnd()) {
            QString line = m_file.readLine();
            if (!line.startsWith(clientId + ':'))
                data += m_file.readLine();
            else
                clientFound = true;
        }
    } else {
        // error
    }
    m_file.close();
    if (clientFound) {
        m_file.open(QFile::WriteOnly | QFile::Text);
        QTextStream out(&m_file);
        if (m_file.isWritable()) {
            out << data;
        } else {
            // error
        }
        m_file.close();

        m_keys.remove(clientId);
    }
    return clientFound;
}

ClientIdInterface *ClientIdInterface::instance()
{
    return m_instance;
}

const QList<QString> ClientIdInterface::getIdList() const
{
    return m_keys.keys();
}

const QList<QString> ClientIdInterface::getKeyList() const
{
    return m_keys.values();
}

void ClientIdInterface::parseFile()
{
    QMutexLocker locker(&m_mutex);
    m_file.open(QFile::ReadOnly | QFile::Text);
    int bufferLen = MAXLEN*2+2;
    char* buffer = new char[bufferLen];
    if (m_file.isReadable()) {
        while (!m_file.atEnd()) {
            int strLen = m_file.readLine(buffer, bufferLen);
            if (strLen != -1) {
                int delimPos = getDelimPos(buffer, strLen, ':');
                if (delimPos != -1) {
                    QString id = QString::fromLocal8Bit(buffer, delimPos);
                    QString key = QString::fromLocal8Bit(buffer+delimPos+1,
                                                     strLen-delimPos);
                    m_keys.insert(id, key);
                } else {
                // error
                }
            } else {
                // error
            }

        }
    }
    m_file.close();
    delete[] buffer;
}

void ClientIdInterface::saveKeyToFile(const QString& cliendId,
                                      const QString& clientKey)
{
    QMutexLocker locker(&m_mutex);
    m_file.open(QFile::ReadOnly | QFile::Text);
    QString data;
    QString idKey = cliendId + ':';
    while (m_file.canReadLine()) {
        QString line = m_file.readLine();
        if (!line.startsWith(idKey))
            data += line;
    }
    data = data + idKey + clientKey + '\n';
    m_file.close();
    m_file.open(QFile::WriteOnly | QFile::Text);
    QTextStream out(&m_file);
    if (m_file.isWritable()) {
        out << data;
    } else {
        // error
    }
    m_file.close();
}

int ClientIdInterface::getDelimPos(const char *str, const int strLen,
                                   const char delim)
{
//    const char* endStr = str+strLen;
    const char* strIter = str+strLen-1;
    int pos = strLen-1;
    while (strIter >= str) {
        if ((*strIter) == delim)
            return pos;
        else {
            pos--;
            strIter--;
        }
    }
    return -1;
}
