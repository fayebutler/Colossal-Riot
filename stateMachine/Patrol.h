#ifndef PATROL_H
#define PATROL_H

#include "State.h"
#include "Police.h"

class Patrol : public State<Police>
{
private:
    Patrol();

public:
    static Patrol* Instance();

    virtual void Enter(Police*);
    virtual void Execute(Police*);
    virtual void Exit(Police*);
    virtual ~Patrol(){}
};

#endif // PATROL_H
