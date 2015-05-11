#ifndef MOVINGENTITY_H
#define MOVINGENTITY_H

//----------------------------------------------------------------------------------------------------------------------------
/// @file MovingEntity.h
/// @brief defines a moving entity
/// Modified from :-
/// Matt Buckland (2005) Programming AI By Example [Book & Source Code]
/// [Accessed 2015] Available from: https://github.com/wangchen/Programming-Game-AI-by-Example-src
/// Created for use of expansion, for example, creating projectiles etc
//----------------------------------------------------------------------------------------------------------------------------

#include "BaseGameEntity.h"

//----------------------------------------------------------------------------------------------------------------------
/// @class Moving Entity "include/MovingEntity.h"
/// @brief The base class for all moving entities
/// @author Faye Butler, Will Herbert
/// @version 1.0
/// @date Last revision 10/05/2015 Updated to comply with doxygen and NCCA coding standard
//----------------------------------------------------------------------------------------------------------------------

class MovingEntity: public BaseGameEntity
{  
public:
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Moving entity ctor
  /// @param[in] world: Pointer to the gameworld
  /// @param[in] pos: starting position of the entity
  /// @param[in] r: radius of the entity
  /// @param[in] velocity: starting velocity of the entity
  /// @param[in] max_speed: the maximum speed of the entity
  /// @param[in] heading: the initial heading of the entity
  /// @param[in] mass: sets the mass of the entity
  /// @param[in] turn_rate: sets the turn rate of the entity
  /// @param[in] max_force: sets the maximum force of the entity
  //----------------------------------------------------------------------------------------------------------------------------
  MovingEntity(GameWorld* world, ngl::Vec3 pos, float r, ngl::Vec3 velocity, float max_speed, ngl::Vec3 heading, float mass, float turn_rate, float max_force);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the velocity of the entity
  /// @return Vec3: returns vector of the entity
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getVelocity()const{return m_velocity;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the velocity of the entity
  /// @param[in] newVel: new vector to set velocity to
  //----------------------------------------------------------------------------------------------------------------------------
  void setVelocity(ngl::Vec3 newVel){m_velocity = newVel;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the heading of the entity
  /// @return Vec3: returns vector of the heading
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getHeading()const{return m_heading;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the heading of the entity
  /// @param[in] newHeading: new vector to set heading to
  //----------------------------------------------------------------------------------------------------------------------------
  void setHeading(ngl::Vec3 newHeading){m_heading = newHeading;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the side vector of the entity
  /// @return Vec3: returns vector of the side
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getSide()const{return m_side;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the mass of the entity
  /// @return Float: returns the mass
  //----------------------------------------------------------------------------------------------------------------------------
  float getMass()const{return m_mass;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the mass of the entity
  /// @param[in] mass: float to set the mass to
  //----------------------------------------------------------------------------------------------------------------------------
  void setMass(float mass){m_mass = mass;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the maximum speed of the entity
  /// @return Float: the maximum speed
  //----------------------------------------------------------------------------------------------------------------------------
  float getMaxSpeed()const{return m_maxSpeed;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the maximum speed of the entity
  /// @param[in] max_speed = float to set the maximum speed to
  //----------------------------------------------------------------------------------------------------------------------------
  void setMaxSpeed(float max_speed){m_maxSpeed = max_speed;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the maximum force of the entity
  /// @return Float: the maximum force
  //----------------------------------------------------------------------------------------------------------------------------
  float getMaxForce()const{return m_maxForce;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the maximum force of the entity
  /// @param[in] max_force: float to set the maximum force to
  //----------------------------------------------------------------------------------------------------------------------------
  void setMaxForce(float max_force){m_maxForce = max_force;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the maximum turn rate of the entity
  /// @return Float: the maximum turn rate
  //----------------------------------------------------------------------------------------------------------------------------
  float getMaxTurnRate()const{return m_maxTurnRate;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the maximum turn rate of the entity
  /// @param[in] turn_rate: float to set the maximum turn rate to
  //----------------------------------------------------------------------------------------------------------------------------
  void setMaxTurnRate(float turn_rate){m_maxTurnRate = turn_rate;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the speed of the entity
  /// @return Float: the length of the velocity (speed)
  //----------------------------------------------------------------------------------------------------------------------------
  float getSpeed()const{return m_velocity.length();}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Virtual function to define how messages are handled
  /// @param[in] _message: the message recieved by the entity
  //----------------------------------------------------------------------------------------------------------------------------
  virtual bool handleMessage(const Message& _message);

protected:
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Vector of the velocity of the entity
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_velocity;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Float for the maximum speed of the entity
  //----------------------------------------------------------------------------------------------------------------------------
  float m_maxSpeed;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Vec3 for the heading of the entity
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_heading;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Float for the mass of the entity
  //----------------------------------------------------------------------------------------------------------------------------
  float m_mass;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Float for the maximum turn rate of the entity
  //----------------------------------------------------------------------------------------------------------------------------
  float m_maxTurnRate;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Float for the maximum force of the entity
  //----------------------------------------------------------------------------------------------------------------------------
  float m_maxForce;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Vector for the side of the entity
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_side;

};

#endif // MOVINGENTITY_H
