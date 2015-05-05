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
                 float scale): MovingEntity(world,
                                            position,
                                            scale,
                                            velocity,
                                            max_speed,
                                            ngl::Vec3(sin(rotation),0,-cos(rotation)),
                                            mass,
                                            max_turnrate,
                                            max_force),
                                m_timeElapsed(0.0)
{
   m_steering =  new SteeringBehaviour(this);
   m_nextSlot = 0;
   m_smoothingOn = true;
   m_sampleSize = 25.0;
   for(int i =0; i<m_sampleSize; i++)
   {
      ngl::Vec3 zeroValue = ngl::Vec3(0,0,0);
      m_headingHistory.push_back(zeroValue);
   }

}

Vehicle::~Vehicle()
{
    delete m_steering;
}

void Vehicle::update(double time_elapsed)
{

   m_timeElapsed = time_elapsed;

   ngl::Vec3 SteeringForce;
   SteeringForce = m_steering->calculatePrioritizedSum();



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

    if(m_smoothingOn == true);
    {
        m_smoothHeading = smoothingUpdate(getHeading());
        m_heading = m_smoothHeading;
    }

}

bool Vehicle::handleMessage(const Message& _message)
{
  return MovingEntity::handleMessage(_message);
}

void Vehicle::render()
{

}

ngl::Vec3 Vehicle::smoothingUpdate(ngl::Vec3 m_recentHeading)
{
    m_headingHistory[m_nextSlot++] = m_recentHeading;

    if(m_nextSlot == m_headingHistory.size())
    {
        m_nextSlot = 0;
    }

    ngl::Vec3 sum = ngl::Vec3(0,0,0);
    std::vector<ngl::Vec3>::iterator it = m_headingHistory.begin();

    for(it; it!=m_headingHistory.end(); it++)
    {
        sum += *it;
    }

    return sum / m_headingHistory.size();
}







