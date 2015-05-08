#ifndef RIOTER_H
#define RIOTER_H

#include "Agent.h"
#include "StateMachine.h"


class Rioter : public Agent
{
public:
    Rioter(GameWorld* world);
    ~Rioter();

    lua_State* getLuaState() const { return L ; }

    void update(double timeElapsed, double currentTime);
    void draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX);
    void loadMatricesToShader(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX);


    StateMachine<Rioter>* getStateMachine() const { return m_stateMachine; }

    void findTargetID(float _health);

    bool handleMessage(const Message &_message);

    void attack();

    void registerClass(lua_State* _L);

    float getPoliceInfluence()const{return m_policeInfluence;}



protected:
    MessageManager* m_messageMgr;
    StateMachine<Rioter>* m_stateMachine;

    float m_policeInfluence;
};

#endif // RIOTER_H
