#ifndef AGENT_H
#define AGENT_H
#include "physWorld.h"


class agent: public physicsWorld
{
    public:
        agent();
        ~agent();
        btRigidBody *agentRigidBody(btScalar _mass, btVector3 _pos, btCollisionShape* _colShape );

    private:

};

#endif // AGENT_H
