#ifndef NEWKEYCHANGE_H
#define NEWKEYCHANGE_H

#include "abstractidkeychange.h"

class NewKeyChange : public AbstractIdKeyChange
{
public:
    NewKeyChange(const QString& clientId, const QString& newKey);

    void applyChange(ClientIdInterface* iface);

private:
	const QString m_clientId;
    const QString m_newKey;
};

#endif // NEWKEYCHANGE_H
