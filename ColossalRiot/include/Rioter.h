#ifndef RIOTER_H
#define RIOTER_H

#include "Agent.h"
#include "StateMachine.h"


class Rioter : public Agent
{
public:
    Rioter(GameWorld* world, ngl::Obj* _mesh);
    ~Rioter();

    lua_State* getLuaState() const { return L ; }

    void update(double timeElapsed, double currentTime);
    void draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX);
    void loadMatricesToShader(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX);


    StateMachine<Rioter>* getStateMachine() const { return m_stateMachine; }

    void findTargetID(float _health);

    bool handleMessage(const Message &_message);

    void attack();
    void death();

    void protestCohesion(double weight);

    void registerClass(lua_State* _L);

    float getPoliceInfluence()const{return m_policeInfluence;}

    ngl::Vec3 getProtestPos()const{return m_protestPos;}
    void setProtestPos(ngl::Vec3 newPos) {m_protestPos = newPos;}

    void findPathHome();

protected:

    //GameWorld* m_gameworld;
    MessageManager* m_messageMgr;
    StateMachine<Rioter>* m_stateMachine;

    ngl::Vec3 m_protestPos;

    float m_policeInfluence;

};

#endif // RIOTER_H
