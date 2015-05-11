//----------------------------------------------------------------------------------------------------------------------------
/// @file MovingEntity.cpp
/// @brief defines a moving entity
/// Modified from :-
/// Matt Buckland (2005) Programming AI By Example [Book & Source Code]
/// [Accessed 2015] Available from: https://github.com/wangchen/Programming-Game-AI-by-Example-src
//----------------------------------------------------------------------------------------------------------------------------

#include "include/MovingEntity.h"

//----------------------------------------------------------------------------------------------------------------------------
MovingEntity::MovingEntity(GameWorld *world,
                           ngl::Vec3 pos,
                           float r,
                           ngl::Vec3 velocity,
                           float max_speed,
                           ngl::Vec3 heading,
                           float mass,
                           float turn_rate,
                           float max_force):BaseGameEntity(world, typeMovingEntity,pos,r),
                                            m_velocity(velocity),
                                            m_maxSpeed(max_speed),
                                            m_heading(heading),
                                            m_mass(mass),
                                            m_maxTurnRate(turn_rate),
                                            m_maxForce(max_force),
                                            m_side(m_heading.cross(ngl::Vec3(0,1,0)))
{

}

//----------------------------------------------------------------------------------------------------------------------------
bool MovingEntity::handleMessage(const Message& _message)
{
  return BaseGameEntity::handleMessage(_message);
}
