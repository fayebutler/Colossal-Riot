#ifndef POLICE_H
#define POLICE_H

#include "Agent.h"
#include "StateMachine.h"

class Police : public Agent
{
public:
    Police(GameWorld* world);
    ~Police();

    lua_State* getLuaState() const { return L ; }

    void update(double timeElapsed);

    StateMachine<Police>* getStateMachine() const { return m_stateMachine; }

    //bool handleMessage(const Message &_message);

    void attack(int _ID);

    void registerClass(lua_State* _L);

protected:
    StateMachine<Police>* m_stateMachine;
};

#endif // POLICE_H
