#include "agent.h"

agent::agent()
{

}

agent::~agent()
{

}

btRigidBody * agent::agentRigidBody(btScalar _mass, btVector3 _pos, btCollisionShape* _colShape)
{
    //create agent rigid body
    //should agent be kinematic ?
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(_pos.x(), _pos.y(), _pos.z()));
    btDefaultMotionState* agentMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo agentRBInfo(_mass, agentMotionState, _colShape, btVector3(0,0,0));
    btRigidBody* agentRigidBody = new btRigidBody(agentRBInfo);
    m_dynamicsWorld->addRigidBody(agentRigidBody);
    return agentRigidBody;
}




