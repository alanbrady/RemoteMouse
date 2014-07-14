#include "clientidinterface.h"

ClientIdInterface* ClientIdInterface::m_instance = new ClientIdInterface();

ClientIdInterface::ClientIdInterface()
{
    m_keyChars = "abcdefghijklmopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789@!?$";
    m_keyCharLen = strlen(m_keyChars);
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
//    QString data;
    QVector<QString> data;
    if (m_file.isReadable()) {
        while (!m_file.atEnd()) {
            QString line = m_file.readLine();
            if (!line.startsWith(clientId + ':'))
                data.append(line);
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
            QVector<QString>::iterator iter;
            for (iter = data.begin(); iter != data.end(); iter++) {
                out << *iter;
            }
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
    QVector<QString> data;
    QString idKey = cliendId + ':';
    while (!m_file.atEnd()) {
        QString line = m_file.readLine();
        if (!line.startsWith(idKey) && !line.isEmpty()) {
//            data.append(line);
            data.append(line);
        }
    }
    data.append(idKey + clientKey + '\n');
    m_file.close();
    m_file.open(QFile::WriteOnly | QFile::Text);
    QTextStream out(&m_file);
    if (m_file.isWritable()) {
        QVector<QString>::iterator iter;
        for (iter = data.begin(); iter != data.end(); iter++) {
            out << *iter;
        }
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
