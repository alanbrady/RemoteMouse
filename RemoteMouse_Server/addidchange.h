#ifndef ADDIDCHANGE_H
#define ADDIDCHANGE_H

#include "abstractidkeychange.h"

class AddIdChange : public AbstractIdKeyChange
{
public:
    AddIdChange(const QString& clientId, const QString& clientKey);

    void applyChange(ClientIdInterface* iface);

private:
    const QString m_clientId;
    const QString m_clientKey;
};

#endif // ADDIDCHANGE_H
