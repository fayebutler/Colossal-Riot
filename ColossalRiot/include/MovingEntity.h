#ifndef MOVINGENTITY_H
#define MOVINGENTITY_H

#include <ngl/Transformation.h>
#include <include/BaseGameEntity.h>

class MovingEntity: public BaseGameEntity
{
protected:
    ngl::Vec3 m_velocity;
    ngl::Vec3 m_heading;
    ngl::Vec3 m_side;
    float m_mass;
    float m_maxSpeed;
    float m_maxForce;
    float m_maxTurnRate;

public:
    MovingEntity();
    MovingEntity(ngl::Vec3 pos,
                 float r,
                 ngl::Vec3 velocity,
                 float max_speed,
                 ngl::Vec3 heading,
                 float mass,
                 float turn_rate,
                 float max_force):BaseGameEntity(0,pos,r),
                                  m_velocity(velocity),
                                  m_heading(heading),
                                  m_mass(mass),
                                  m_side(m_heading.cross(ngl::Vec3(0,1,0))),
                                  m_maxSpeed(max_speed),
                                  m_maxForce(max_force),
                                  m_maxTurnRate(turn_rate)
    {}



    ngl::Vec3 getVelocity()const{return m_velocity;}
    void setVelocity(ngl::Vec3 newVel){m_velocity = newVel;}

    ngl::Vec3 getHeading()const{return m_heading;}
    void setHeading(ngl::Vec3 newHeading){m_heading = newHeading;}

    ngl::Vec3 getSide()const{return m_side;}

    float getMass()const{return m_mass;}
    void setMass(float mass){m_mass = mass;}

    float getMaxSpeed()const{return m_maxSpeed;}
    void setMaxSpeed(float max_speed){m_maxSpeed = max_speed;}

    float getMaxForce()const{return m_maxForce;}
    void setMaxForce(float max_force){m_maxForce = max_force;}

    float getMaxTurnRate()const{return m_maxTurnRate;}
    void setMaxTurnRate(float turn_rate){m_maxTurnRate = turn_rate;}

    float getSpeed()const{return m_velocity.length();}

    virtual bool handleMessage(const Message& _message);



};

#endif // MOVINGENTITY_H
