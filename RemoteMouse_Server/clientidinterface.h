#ifndef CLIENTIDINTERFACE_H
#define CLIENTIDINTERFACE_H

#include <QString>
#include <QFile>
#include <QHash>

#define MAXLEN 30

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
