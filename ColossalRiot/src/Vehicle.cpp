//----------------------------------------------------------------------------------------------------------------------------
/// @file Vehicle.cpp
/// @brief defines a vehicle
/// Modified from :-
/// Matt Buckland (2005) Programming AI By Example [Book & Source Code]
/// [Accessed 2015] Available from: https://github.com/wangchen/Programming-Game-AI-by-Example-src
//----------------------------------------------------------------------------------------------------------------------------

#include "Vehicle.h"
#include "GameWorld.h"

//----------------------------------------------------------------------------------------------------------------------------
/// Ctor that intialises variables
//----------------------------------------------------------------------------------------------------------------------------
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
  m_pathIndex = 0;
  m_crosshair= ngl::Vec3(0,0,0);
  m_squadCrosshair = ngl::Vec3(0,0,0);

  for(int i =0; i<m_sampleSize; i++)
  {
    ngl::Vec3 zeroValue = ngl::Vec3(0,0,0);
    m_headingHistory.push_back(zeroValue);
  }
}

//----------------------------------------------------------------------------------------------------------------------------
Vehicle::~Vehicle()
{
  delete m_steering;
}

//----------------------------------------------------------------------------------------------------------------------------
/// Update Function - used to calculate the acceleration, using the steering force and mass,
///                   this is multiplied by the time elapsed from the last tick to create the new vehicle position
///                 - Smoothing is also added to the heading, so that the jumpy erratic movement is removed
///                 - finally if a path has been set the vehicle is set to follow it
//----------------------------------------------------------------------------------------------------------------------------
void Vehicle::update(double time_elapsed)
{
  m_timeElapsed = time_elapsed;

  ngl::Vec3 SteeringForce;
  SteeringForce = m_steering->calculatePrioritizedSum();

  ngl::Vec3 acceleration = SteeringForce / m_mass;
  m_velocity += acceleration * time_elapsed;

  //velocity truncated by maxspeed

  if(m_velocity.lengthSquared() > m_maxSpeed * m_maxSpeed)
  {
    if(m_velocity.lengthSquared() == 0.0f)
    {
      std::cout<<"Velocity in vehicle update equals zero, can't normalise"<<std::endl;
    }
    else
    {
      m_velocity.normalize();
    }
   m_velocity = m_velocity * m_maxSpeed;
  }

  m_pos += m_velocity * time_elapsed;

  if(m_velocity.lengthSquared()>0.000000001)
  {
    ngl::Vec3 tempVel;
    tempVel = m_velocity;

    if(tempVel.lengthSquared() == 0.0f)
    {
      std::cout<<"tempVel in vehicle update equals zero, can't normalise"<<std::endl;
    }
    else
    {
      tempVel.normalize();
    }

    m_heading = tempVel;

    m_side = m_heading.cross(ngl::Vec3(0,1,0));
  }

  //smooth the vehicles heading
  if(m_smoothingOn == true);
  {
    m_smoothHeading = smoothingUpdate(getHeading());
    m_heading = m_smoothHeading;
  }

  // if the path has been set, set the vehicle to follow it
  if(m_path.size() != 0)
  {
    this->followPath();
  }

}

//----------------------------------------------------------------------------------------------------------------------------
bool Vehicle::handleMessage(const Message& _message)
{
  return MovingEntity::handleMessage(_message);
}

//----------------------------------------------------------------------------------------------------------------------------
/// Smoothing - calculates the average heading from a set number of past headings (_sampleSize)
//----------------------------------------------------------------------------------------------------------------------------
ngl::Vec3 Vehicle::smoothingUpdate(ngl::Vec3 _recentHeading)
{
  m_headingHistory[m_nextSlot++] = _recentHeading;

  if(m_nextSlot == m_headingHistory.size())
  {
    m_nextSlot = 0;
  }

  ngl::Vec3 sum = ngl::Vec3(0,0,0);

  for(std::vector<ngl::Vec3>::iterator it = m_headingHistory.begin(); it!=m_headingHistory.end(); it++)
  {
    sum += *it;
  }

  return sum / m_headingHistory.size();
}

//----------------------------------------------------------------------------------------------------------------------------
/// Find Exit - given a vector of vectors, this function iterates through and finds the closest point to the vehicle
///           - mainly used for rioters finding their way home
///           - using the returned vector, a path can be found to that point
//----------------------------------------------------------------------------------------------------------------------------
ngl::Vec3 Vehicle::findNearestExit(std::vector<ngl::Vec3> _exits)
{
  //find nearest point to current positiong and set that as path thing

  ngl::Vec3 bestExit;
  float dist = 1000000000.0;
  int exitSize = _exits.size();
  for(int i = 0; i<exitSize; i++)
  {
    ngl::Vec3 currentExit = _exits[i];
    float testDist = (currentExit- this->getPos()).lengthSquared();
    if(testDist < dist*dist)
    {
      bestExit = currentExit;
      dist = testDist;
    }
  }
  return bestExit;
}

//----------------------------------------------------------------------------------------------------------------------------
/// Find Path - sets m_path to be a path to the inputted target, if this path is not zero then the vehicle is set to follow
//----------------------------------------------------------------------------------------------------------------------------
void Vehicle::findPath(ngl::Vec3 _target)
{
  m_path.clear();
  m_pathIndex =0;
  std::vector<ngl::Vec3> path;
  path = m_world->getCellGraph()->findPath(this, _target);
  m_path = path;
  m_crosshair = m_path[m_pathIndex];
}

//----------------------------------------------------------------------------------------------------------------------------
/// Follow Path - function iterates through the vectors in the path, and once the vehicle has reached the current crosshair,
///               the next one is set as the destination
//----------------------------------------------------------------------------------------------------------------------------
void Vehicle::followPath()
{
  //go through each vec3 and set as crosshair to follow the path

  if (m_pathIndex != m_path.size()-1 && (this->getPos() - m_path[m_pathIndex]).lengthSquared()<= 9)
  {
    setPathIndex(m_pathIndex += 1);
    m_crosshair = m_path[m_pathIndex];
  }
  if(m_pathIndex == m_path.size()-1)
  {
    m_path.clear();
    m_pathIndex =0;
  }
}






