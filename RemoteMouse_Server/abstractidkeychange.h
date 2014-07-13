#ifndef ABSTRACTIDKEYCHANGE_H
#define ABSTRACTIDKEYCHANGE_H

#include "clientidinterface.h"

class AbstractIdKeyChange
{
public:
    AbstractIdKeyChange() {}
    virtual ~AbstractIdKeyChange() {}

    virtual void applyChange(ClientIdInterface*) = 0;
};

#endif // ABSTRACTIDKEYCHANGE_H
