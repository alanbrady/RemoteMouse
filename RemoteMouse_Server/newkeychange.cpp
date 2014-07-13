#include "newkeychange.h"

NewKeyChange::NewKeyChange(const QString &clientId, const QString &newKey)
    : m_clientId(clientId), m_newKey(newKey)
{
}

void NewKeyChange::applyChange(ClientIdInterface *iface)
{
    iface->setKeyForClient(m_clientId, m_newKey);
}
