#ifndef DELETEIDCHANGE_H
#define DELETEIDCHANGE_H

#include "abstractidkeychange.h"

class DeleteIdChange : public AbstractIdKeyChange
{
public:
    DeleteIdChange(const QString& idToDelete);

    void applyChange(ClientIdInterface* iface);

private:
    const QString m_idToDelete;
};

#endif // DELETEIDCHANGE_H
