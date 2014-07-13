#ifndef RENAMEIDCHANGE_H
#define RENAMEIDCHANGE_H

#include "abstractidkeychange.h"

class RenameIdChange : public AbstractIdKeyChange
{
public:
    RenameIdChange(const QString& oldId, const QString& newId, const QString& key);

    void applyChange(ClientIdInterface* iface);

private:
    const QString m_oldId;
    const QString m_newId;
    const QString m_key;
};

#endif // RENAMEIDCHANGE_H
