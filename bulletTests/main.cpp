#include "physWorld.h"
#include "agent.h"

int main()
{
    physicsWorld* m_physics;
    m_physics = new physicsWorld;
    m_physics->setGravity(0,-10,0);
    m_physics->addGroundPlane();

    btCollisionShape * colShape = m_physics->addAgentColShape(1.f, 5.f);

    agent a;
    a.agentRigidBody(2, btVector3(0,0,0), colShape);

    m_physics->step(1.0/60.0,10);


}
