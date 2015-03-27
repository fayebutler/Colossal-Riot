#ifndef RIOTER_H
#define RIOTER_H

#include "BaseEntity.h"
#include "StateMachine.h"

class Rioter : public BaseEntity
{
public:
    Rioter();
    ~Rioter();

    lua_State* getLuaState() const { return L ; }

    void update();
    StateMachine<Rioter>* getStateMachine() const { return m_stateMachine; }

    bool handleMessage(const Message &_message);

    void attack(int _ID);

    void registerClass(lua_State* _L);

protected:
    StateMachine<Rioter>* m_stateMachine;
};

#endif // RIOTER_H
