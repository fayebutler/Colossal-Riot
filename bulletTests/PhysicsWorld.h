#ifndef PHYSWORLD_H
#define PHYSWORLD_H

#include "btBulletDynamicsCommon.h"


class physicsWorld
{
    public:
        physicsWorld();
        ~physicsWorld();
        inline void setGravity(float _x, float _y, float _z)
        {
            m_dynamicsWorld->setGravity(btVector3(_x, _y, _z));
        }
        inline void step(float _time, float _step)
        {
            m_dynamicsWorld->stepSimulation(_time, _step);
        }

        void addGroundPlane();
        btCollisionShape * addAgentColShape(float _rad, float _height);



    protected:
        btBroadphaseInterface * m_broadphase;
        btDefaultCollisionConfiguration * m_collisionConfiguration;
        btCollisionDispatcher * m_dispatcher;
        btSequentialImpulseConstraintSolver * m_solver;
        btDiscreteDynamicsWorld * m_dynamicsWorld;




};

#endif // PHYSWORLD_H
