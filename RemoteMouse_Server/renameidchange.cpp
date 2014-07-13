#include "renameidchange.h"

RenameIdChange::RenameIdChange(const QString &oldId, const QString &newId, const QString &key)
    : m_oldId(oldId), m_newId(newId), m_key(key)
{
}

void RenameIdChange::applyChange(ClientIdInterface *iface)
{
    iface->removeClient(m_oldId);
    iface->setKeyForClient(m_newId, m_key);
}
