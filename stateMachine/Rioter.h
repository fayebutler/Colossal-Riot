#ifndef RIOTER_H
#define RIOTER_H

#include "BaseEntity.h"
#include "StateMachine.h"

class Rioter : public BaseEntity
{
public:
    Rioter();
    ~Rioter();

    lua_State* getLuaState() { return L ; }

    void update();
    StateMachine<Rioter>* getStateMachine() const { return m_stateMachine; }

    bool handleMessage(const Message &_message);

    static void attack(int _ID);
    static int cpp_attack(lua_State* L);


protected:
    StateMachine<Rioter>* m_stateMachine;
    lua_State *L;
};

#endif // RIOTER_H
