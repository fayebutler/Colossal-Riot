#include "Vehicle.h"
#include "math.h"



Vehicle::Vehicle(GameWorld* world,
                 ngl::Vec3 position,
                 ngl::Vec3 velocity,
                 float rotation,
                 float mass,
                 float max_force,
                 float max_speed,
                 float max_turnrate,
                 float scale): MovingEntity(position,
                                            scale,
                                            velocity,
                                            max_speed,
                                            ngl::Vec3(sin(rotation),0,-cos(rotation)),
                                            mass,
                                            max_turnrate,
                                            max_force),
                                m_world(world),
                                m_timeElapsed(0.0)
{
   m_steering =  new SteeringBehaviour(this);


}

void Vehicle::update(double time_elapsed)
{

   m_timeElapsed = time_elapsed;
//   std::cout<<"time  "<<m_timeElapsed<<std::endl;


   ngl::Vec3 oldPos = getPos();

   ngl::Vec3 SteeringForce;
   SteeringForce = m_steering->calculateWeightedSum();

   ngl::Vec3 acceleration = SteeringForce / m_mass;
   m_velocity += acceleration * time_elapsed;

   //velocity truncate by maxspeed

   if(m_velocity.length() > m_maxSpeed)
   {
       m_velocity.normalize();
       m_velocity = m_velocity * m_maxSpeed;
   }


   m_pos += m_velocity * time_elapsed;

   if(m_velocity.lengthSquared()>0.000000001)
   {

       ngl::Vec3 tempVel;
       tempVel = m_velocity;
       tempVel.normalize();
       m_heading = tempVel;
       m_side = m_heading.cross(ngl::Vec3(0,1,0));
   }

}

bool Vehicle::handleMessage(const Message& _message)
{
  MovingEntity::handleMessage(_message);
}

void Vehicle::render()
{

}
