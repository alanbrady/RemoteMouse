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

const QByteArray ClientIdInterface::getKeyForClient(const QString &clientId)
{
    QMutexLocker locker(&m_mutex);
    QByteArray key = m_keys.value(clientId, "Error: ID does not exist!");
    return key;
}

const QByteArray ClientIdInterface::generateNewKey() const
{
    QByteArray newKey(m_keyCharLen, '\0');
    for (int i = 0; i < KEY_LEN; i++) {
        newKey[i] = m_keyChars[rand()%m_keyCharLen];
    }
    return newKey;
}

void ClientIdInterface::setKeyForClient(const QString &clientId,
                                        const QByteArray &clientKey)
{
    m_keys.insert(clientId, clientKey);
    saveKeyToFile(clientId, clientKey);
}

ClientIdInterface *ClientIdInterface::instance()
{
    return m_instance;
}

void ClientIdInterface::parseFile()
{
    QMutexLocker locker(&m_mutex);
    m_file.open(QFile::ReadOnly);
    int bufferLen = MAXLEN*2+2;
    char* buffer = new char[bufferLen];
    while (m_file.isReadable()) {
        int strLen = m_file.readLine(buffer, bufferLen);
        if (strLen != -1) {
            int delimPos = getDelimPos(buffer, strLen, ':');
            if (delimPos != -1) {
                QString id = QString::fromLocal8Bit(buffer, delimPos);
                QByteArray key = QByteArray::fromRawData(buffer+delimPos+1,
                                                     strLen-delimPos);
                m_keys.insert(id, key);
            } else {
                // error
            }
        } else {
            // error
        }

    }
    m_file.close();
    delete[] buffer;
}

void ClientIdInterface::saveKeyToFile(const QString& cliendId,
                                      const QByteArray& clientKey)
{
    // TODO: write over existing ID
    QMutexLocker locker(&m_mutex);
    m_file.open(QFile::Append);
    m_file.write(cliendId.toStdString().c_str(), MAXLEN);
    m_file.write(":", 1);
    m_file.write(clientKey, MAXLEN);
    m_file.write("\n", 1);
    m_file.close();
}

int ClientIdInterface::getDelimPos(const char *str, const int strLen,
                                   const char delim)
{
    const char* endStr = str+strLen;
    int pos = 0;
    while (str != endStr) {
        if ((*str) == delim)
            return pos;
        else {
            pos++;
            str++;
        }
    }
    return -1;
}


