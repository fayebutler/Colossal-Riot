#ifndef POLICE_H
#define POLICE_H

#include "Agent.h"
#include "StateMachine.h"
#include "MessageManager.h"

class Police : public Agent
{
public:
    Police(GameWorld* world);
    ~Police();

    lua_State* getLuaState() const { return L ; }

    void update(double timeElapsed, double currentTime);
    void draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX);
    void loadMatricesToShader(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX);


    StateMachine<Police>* getStateMachine() const { return m_stateMachine; }

    bool handleMessage(const Message &_message);

    void attack();

    void registerClass(lua_State* _L);


protected:
    MessageManager* m_messageMgr;

    StateMachine<Police>* m_stateMachine;
};

#endif // POLICE_H
