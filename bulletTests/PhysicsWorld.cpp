#include "physWorld.h"


physicsWorld::physicsWorld()
{
    //initalize physics
    m_broadphase = new btDbvtBroadphase();
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_solver = new btSequentialImpulseConstraintSolver;
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

}

physicsWorld::~physicsWorld()
{
    //delete physics
    delete m_broadphase;
    delete m_collisionConfiguration;
    delete m_dispatcher;
    delete m_solver;
    delete m_dynamicsWorld;

}

void physicsWorld::addGroundPlane()
{
    //ground plane collision shape and rigid body (mass 0 so is static and can't be moved)
    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0), 1);
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,-1,0)));
    btRigidBody::btRigidBodyConstructionInfo groundRBInfo(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRBInfo);
    m_dynamicsWorld->addRigidBody(groundRigidBody);

}

btCollisionShape * physicsWorld::addAgentColShape(float _rad, float _height)
{
    //create collision shape for agent
    //simple cylinder surrounding, will be large to callback when near collision and affect avoidance
    btCollisionShape* agentShape = new btCylinderShape(btVector3(_rad, _height/2.0, _rad));
    return agentShape;
}


