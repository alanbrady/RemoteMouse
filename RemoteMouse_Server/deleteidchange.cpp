#include "deleteidchange.h"

DeleteIdChange::DeleteIdChange(const QString &idToDelete)
    : m_idToDelete(idToDelete)
{
}

void DeleteIdChange::applyChange(ClientIdInterface *iface)
{
    iface->removeClient(m_idToDelete);
}
