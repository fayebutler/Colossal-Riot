#include "include/MovingEntity.h"



MovingEntity::MovingEntity()
{

}

bool MovingEntity::handleMessage(const Message& _message)
{
  BaseGameEntity::handleMessage(_message);
}

//MovingEntity::MovingEntity(ngl::Vec3 pos, float r, ngl::Vec3 velocity, float max_speed, ngl::Vec3 heading, float mass, float turn_rate, float max_force)
//{




//    m_velocity = velocity;
//    m_heading = heading;
//    m_mass = mass;
//    m_side = m_heading.cross(ngl::Vec3(0,1,0));
//    m_maxSpeed = max_speed;
//    m_maxForce = max_force;
//    m_maxTurnRate = turn_rate;
//}
