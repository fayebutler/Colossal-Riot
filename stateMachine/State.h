#ifndef STATE_H
#define STATE_H

#include <iostream>

template <class entityType>

class State
{
public:
    //State();
    virtual void enter(entityType*)=0;
    virtual void execute(entityType*)=0;
    virtual void exit(entityType*)=0;
    virtual ~State(){}

};

#endif // STATE_H
