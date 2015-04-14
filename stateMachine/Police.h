#ifndef POLICE_H
#define POLICE_H

#include "BaseEntity.h"
#include "StateMachine.h"

class Police : public BaseEntity
{
public:
    Police();
    ~Police();

    void update();
    StateMachine<Police>* getStateMachine() const { return m_stateMachine; }

    bool handleMessage(const Message &_message);

    lua_State* getLuaState() { return L ; }

protected:
    StateMachine<Police>* m_stateMachine;
    lua_State *L;
};

#endif // POLICE_H
