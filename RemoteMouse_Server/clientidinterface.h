#ifndef CLIENTIDINTERFACE_H
#define CLIENTIDINTERFACE_H

#include <QString>
#include <QByteArray>
#include <QFile>
#include <QHash>
#include <QMutex>
#include <QMutexLocker>
#include <QTextStream>
#include <time.h>
#include <stdlib.h>
#define MAXLEN 50
#define ID_LEN 8
#define KEY_LEN 10

/**
 * @brief ClientIdInterface dictates how client IDs and keys are stored
 * For now IDs and keys are stored in plaintext data files.  Theoretically
 * they should be encrypted somehow, but that's probably a bit out of scope
 * for the project.  The only purpose of the id/key scheme is to
 * ensure only verified clients can connect and control the mouse.
 *
 * The scheme works such that the client generates a random id and key and
 * inputs it into the server so that it knows who it is.
 *
 * ClientIdInterface is a singleton by design
 *
 * TODO:
 *  - check for ID conflicts
 *
 * WISHFUL THINKING:
 *  - encrypt id/key file
 */

class ClientIdInterface
{
public:
    ~ClientIdInterface();
    const QByteArray getKeyForClient(const QString& clientId);
    const QByteArray generateNewKey() const;
    void setKeyForClient(const QString& clientId, const QByteArray& clientKey);
    bool removeClient(const QString& clientId);
    int getIdLen() const { return ID_LEN; }
    static ClientIdInterface* instance();
    const QList<QString> getIdList() const;
    const QList<QByteArray> getKeyList() const;

private:
    // Constructors are made private to enforce singleton, access should be
    // limited to the instance() function
    ClientIdInterface();
    explicit ClientIdInterface(const ClientIdInterface&);
    const ClientIdInterface& operator=(const ClientIdInterface&);

    const char *m_keyChars;
    unsigned int m_keyCharLen;
    QFile m_file;
    QHash<QString, QByteArray> m_keys;
    QMutex m_mutex;

    static ClientIdInterface *m_instance;

    void parseFile();
    void saveKeyToFile(const QString &cliendId, const QByteArray &clientKey);
    int getDelimPos(const char* str, const int strLen, const char delim);
};

#endif // CLIENTIDINTERFACE_H
