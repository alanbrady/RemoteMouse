#ifndef CLIENTIDINTERFACE_H
#define CLIENTIDINTERFACE_H

#include <QString>
#include <QFile>
#include <QHash>

#define MAXLEN 30

/**
 * @brief ClientIdInterface dictates how client id's and keys are stored
 * For now id's and keys are stored in plaintext data files.  Theoretically
 * they should be encrypted somehow, but that's probably a bit out of scope
 * for the project.  The only purpose of the id/key functionality is to
 * ensure only verified clients can connect and control the mouse.
 */

class ClientIdInterface
{
public:
    ClientIdInterface(const QString& path);

    const QString getKeyForClient(const QString& clientId);
    void setKeyForClient(const QString clientId, const QString clientKey);

private:
    QFile file;
    QHash<QString, QString> keys;

    void parseFile();
    void saveKeyToFile(const QString cliendId, const QString clientKey);
    int getDelimPos(const char* str, const int strLen, const char delim);
};

#endif // CLIENTIDINTERFACE_H
