#ifndef POLICE_H
#define POLICE_H

#include "Agent.h"
#include "StateMachine.h"
//#include "MessageManager.h"

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

    void findTargetID(float _health);

    bool handleMessage(const Message &_message);

    void attack();

    void registerClass(lua_State* _L);

    ngl::Vec3 getSquadPos()const{return m_squadPos;}
    void setSquadPos(ngl::Vec3 newPos) {m_squadPos = newPos;}

    float getSquadRadius()const{return m_squadRadius;}
    void setSquadRadius(float newRad) {m_squadRadius = newRad;}

    void squadCohesion(double weight);

    int getSquadID()const{return m_squadID;}
    void setSquadID(int squadID){m_squadID = squadID;}

    bool getIsMoving() const { return m_isMoving; }
    void setIsMoving(bool _val) { m_isMoving = _val; }

    int getPathIndex()const{return m_pathIndex;}
    void setPathIndex(int pathIndex){m_pathIndex = pathIndex;}


protected:
    MessageManager* m_messageMgr;

    StateMachine<Police>* m_stateMachine;

    ngl::Vec3 m_squadPos;
    float m_squadRadius;
    int m_squadID;

    int m_pathIndex;

    bool m_isMoving;

};

#endif // POLICE_H
