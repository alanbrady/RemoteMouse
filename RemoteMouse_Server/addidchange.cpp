#include "addidchange.h"

AddIdChange::AddIdChange(const QString &clientId, const QString &clientKey)
    : m_clientId(clientId), m_clientKey(clientKey)
{
}

void AddIdChange::applyChange(ClientIdInterface *iface)
{
    iface->setKeyForClient(m_clientId, m_clientKey);
}
