#include "clientidinterface.h"

ClientIdInterface::ClientIdInterface(const QString& path)
{
    file.setFileName(path);
}

const QString ClientIdInterface::getKeyForClient(const QString &clientId)
{
    return keys.value(clientId, "error");

}

void ClientIdInterface::setKeyForClient(const QString clientId,
                                        const QString clientKey)
{
    keys.insert(clientId, clientKey);
    saveKeyToFile(clientId, clientKey);
}

void ClientIdInterface::parseFile()
{
    file.open(QFile::ReadOnly);
    int bufferLen = MAXLEN*2+2;
    char* buffer = new char[bufferLen];
    while (file.isReadable()) {
        int strLen = file.readLine(buffer, bufferLen);
        if (strLen != -1) {
            int delimPos = getDelimPos(buffer, strLen, ':');
            if (delimPos != -1) {
                // needs some double checking when I'm not exhausted
                QString id = QString::fromLocal8Bit(buffer, delimPos);
                QString key = QString::fromLocal8Bit(buffer+delimPos+1,
                                                     strLen-delimPos);
                keys.insert(id, key);
            } else {
                // error
            }
        } else {
            // error
        }

    }
    delete [] buffer;
}

void ClientIdInterface::saveKeyToFile(const QString cliendId,
                                      const QString clientKey)
{
    file.open(QFile::Append);
    file.write(cliendId.toStdString().c_str(), MAXLEN);
    file.write(":", 1);
    file.write(clientKey.toStdString().c_str(), MAXLEN);
    file.write("\n", 1);
    file.close();
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


