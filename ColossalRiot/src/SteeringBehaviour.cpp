//----------------------------------------------------------------------------------------------------------------------------
/// @file SteeringBehaviour.cpp
/// @brief defines behaviours and steering forces
/// Modified from :-
/// Matt Buckland (2005) Programming AI By Example [Book & Source Code]
/// [Accessed 2015] Available from: https://github.com/wangchen/Programming-Game-AI-by-Example-src
//----------------------------------------------------------------------------------------------------------------------------

#include "SteeringBehaviour.h"
#include "GameWorld.h"

//----------------------------------------------------------------------------------------------------------------------------
SteeringBehaviour::SteeringBehaviour(Vehicle* _agent):
  m_vehicle(_agent),
  m_activeFlags(0),
  m_wanderDistance(3.0),
  m_wanderRadius(1.0),
  m_wanderJitter(10.0),
  m_weightSeek(1.0),
  m_weightFlee(1.0),
  m_weightArrive(1.0),
  m_weightWander(1.0),
  m_weightCohesion(1.0),
  m_weightSeparation(1.0),
  m_weightAlignment(1.0),
  m_weightObstacleAvoidance(1.0),
  m_weightWallAvoidance(1.0),
  m_weightPursuit(1.0),
  m_weightEvade(1.0)
{
  m_entityMgr = new EntityManager();

  double theta = (float)rand()/RAND_MAX * (M_PI * 2);
  m_wanderTarget = ngl::Vec3(m_wanderRadius * cos(theta), 0, m_wanderRadius * sin(theta));

  m_targetAgent = NULL;
}

//----------------------------------------------------------------------------------------------------------------------------
SteeringBehaviour::~SteeringBehaviour()
{
  //delete m_entityMgr;
}

//----------------------------------------------------------------------------------------------------------------------------
/// Calculates total steering force by ordering behaviours by priority
/// Each force is added to the steering if there is enough max force left over (AccumulateForce function)
//----------------------------------------------------------------------------------------------------------------------------
ngl::Vec3 SteeringBehaviour::calculatePrioritizedSum()
{
  m_steeringForce = ngl::Vec3(0.f, 0.f, 0.f);

  ngl::Vec3 force;

  if(on(wall_avoidance))
  {
    force = WallAvoidance() * m_weightWallAvoidance;

    if(!accumulateForce(m_steeringForce, force))
    {
      return m_steeringForce;
    }
    else
    {
      m_steeringForce += force;
    }
  }

  if(on(obstacle_avoidance))
  {
    force = ObstacleAvoidance() * m_weightObstacleAvoidance;
    if(!accumulateForce(m_steeringForce, force))
    {
      return m_steeringForce;
    }
    else
    {
      m_steeringForce += force;
    }
  }

  if(on(separation))
  {
    force = Separation(m_allNeighbours) * m_weightSeparation;
    if(!accumulateForce(m_steeringForce, force))
    {
      return m_steeringForce;
    }
    else
    {
      m_steeringForce += force;
    }
  }

  if(on(cohesion))
  {
    force = Cohesion(m_friendlyNeighbours) * m_weightCohesion;
    if(!accumulateForce(m_steeringForce, force))
    {
      return m_steeringForce;
    }
    else
    {
      m_steeringForce += force;
    }
  }

  if(on(alignment))
  {
    force = Alignment(m_friendlyNeighbours) * m_weightAlignment;
    if(!accumulateForce(m_steeringForce, force))
    {
      return m_steeringForce;
    }
    else
    {
      m_steeringForce += force;
    }
  }

  if(on(pursuit))
  {
    if (m_targetAgent != NULL)
    {
      force = Pursuit(m_targetAgent) * m_weightPursuit;
      if(!accumulateForce(m_steeringForce, force))
      {
        return m_steeringForce;
      }
      else
      {
        m_steeringForce += force;
      }
    }
  }

  if(on(evade))
  {
    if (m_targetAgent != NULL)
    {
      force = Evade(m_targetAgent) * m_weightEvade;
      if(!accumulateForce(m_steeringForce, force))
      {
        return m_steeringForce;
      }
      else
      {
        m_steeringForce += force;
      }
    }
  }

  if(on(seek))
  {
    force = Seek(m_vehicle->getCrosshair()) * m_weightSeek;
    if(!accumulateForce(m_steeringForce, force))
    {
      return m_steeringForce;
    }
    else
    {
      m_steeringForce += force;
    }
  }

  if(on(flee))
  {
    force = Flee(m_vehicle->getCrosshair()) * m_weightFlee;
    if(!accumulateForce(m_steeringForce, force))
    {
      return m_steeringForce;
    }
    else
    {
      m_steeringForce += force;
    }
  }

  if(on(arrive))
  {
    force = Arrive(m_vehicle->getCrosshair(), 0.1) * m_weightArrive;
    if(!accumulateForce(m_steeringForce, force))
    {
      return m_steeringForce;
    }
    else
    {
      m_steeringForce += force;
    }
  }

  if(on(squad_cohesion))
  {
    force = SquadCohesion(m_vehicle->getSquadCrosshair(), 0.5) * m_weightSquadCohesion;

    if(!accumulateForce(m_steeringForce, force))
    {
      return m_steeringForce;
    }
    else
    {
      m_steeringForce += force;
    }
  }

  if(on(wander))
  {
    force = Wander() * m_weightWander;
    if(!accumulateForce(m_steeringForce, force))
    {
      return m_steeringForce;
    }
    else
    {
      m_steeringForce += force;
    }
  }

  return m_steeringForce;
}

//----------------------------------------------------------------------------------------------------------------------------
/// Adds together steering forces, making sure does not exceed the maximum force
//----------------------------------------------------------------------------------------------------------------------------
bool SteeringBehaviour::accumulateForce(ngl::Vec3 currentTotal, ngl::Vec3 &force)
{
  double magnitude = currentTotal.length();

  double forceLeft =  m_vehicle->getMaxForce() - magnitude;

  if(forceLeft < 0.0)
  {
    return false;
  }
  else if(force.lengthSquared() > (forceLeft * forceLeft))
  {
    if(force.lengthSquared() == 0.0f)
    {
      std::cout<<"Force in accumulate force equals zero, can't normalise"<<std::endl;
    }
    else
    {
      force.normalize();
    }
    force *= m_vehicle->getMaxForce();
    return true;
  }
  else
  {
    return true;
  }
}

//----------------------------------------------------------------------------------------------------------------------------
//behaviour type functions
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
/// Seek - returns the vector toward to the target position
//----------------------------------------------------------------------------------------------------------------------------
ngl::Vec3 SteeringBehaviour::Seek(ngl::Vec3 targetPos)
{
  ngl::Vec3 desiredVelocity = ngl::Vec3(targetPos - m_vehicle->getPos());


  return (desiredVelocity - m_vehicle->getVelocity());
}

//----------------------------------------------------------------------------------------------------------------------------
/// Flee - returns vector directing away from the target position
//----------------------------------------------------------------------------------------------------------------------------
ngl::Vec3 SteeringBehaviour::Flee(ngl::Vec3 targetPos)
{
  ngl::Vec3 desiredVelocity = ngl::Vec3(m_vehicle->getPos() - targetPos);

  return desiredVelocity - m_vehicle->getVelocity();
}

//----------------------------------------------------------------------------------------------------------------------------
/// Arrive - returns vector towards the target position, but as it gets closer the forces lowers
///           so the vehicle decelerates
//----------------------------------------------------------------------------------------------------------------------------
ngl::Vec3 SteeringBehaviour::Arrive(ngl::Vec3 targetPos, int deceleration)
{
  ngl::Vec3 toTarget = targetPos - m_vehicle->getPos();
  double dist = toTarget.length();

  if(dist>0.1)
  {
    double decelerationTweak = 0.01;

    double speed = dist/(deceleration*decelerationTweak);

    //make velocity not exceed maxspeed
    if( speed > m_vehicle->getMaxSpeed())
    {
      speed = m_vehicle->getMaxSpeed();
    }
    ngl::Vec3 desiredVelocity = toTarget * speed/dist;
    return(desiredVelocity - m_vehicle->getVelocity());
  }
  else if(dist <= 0.1 )
  {
    return ngl::Vec3(0,0,0);
  }

  return ngl::Vec3(0,0,0);
}

//----------------------------------------------------------------------------------------------------------------------------
/// Wander - creates a circle around the entity of radius r. a new point on the circle is found by getting the current point
///          adding a vector to the point, normalising the resulting local position vector and multiplying this vector by
///          the radius. The circle is then moved forward in local space by a distance d and the entity creates a steering
///          force from the vector from the entity origin to the point on the circle
//----------------------------------------------------------------------------------------------------------------------------
ngl::Vec3 SteeringBehaviour::Wander()
{
  double jitterTimeSlice = m_wanderJitter * m_vehicle->getTimeElapsed() ;

  float randomClampedX = -1+2*((float)rand())/RAND_MAX;
  float randomClampedZ = -1+2*((float)rand())/RAND_MAX;

  m_wanderTarget += ngl::Vec3(randomClampedX * jitterTimeSlice ,0, randomClampedZ * jitterTimeSlice);

  if(m_wanderTarget.lengthSquared() == 0.0f)
  {
    std::cout<<"Wander Target in wander equals zero, can't normalise"<<std::endl;
  }
  else
  {
    m_wanderTarget.normalize();
  }
  m_wanderTarget *= m_wanderRadius;
  ngl::Vec3 localTarget = m_wanderTarget + ngl::Vec3(m_wanderDistance,0,0);

  // local to world space
  ngl::Vec3 headingNormalise;
  ngl::Vec3 worldNormalise = ngl::Vec3(1,0,0);
  headingNormalise = m_vehicle->getHeading();

  if(headingNormalise.lengthSquared() == 0.0f)
  {
    std::cout<<"heading Normalise in wander equals zero, can't normalise"<<std::endl;
  }
  else
  {
    headingNormalise.normalize();
  }

  double dotProduct = worldNormalise.dot(headingNormalise);

  double magnitude = headingNormalise.length()*worldNormalise.length();
  double temp = dotProduct/magnitude;
  double angle = acos(temp);
  if(headingNormalise.m_z > 0)
  {
    angle = 2*M_PI - angle;
  }

  ngl::Transformation trans;
  trans.setRotation(0, (-angle * 180)/M_PI, 0);
  ngl::Vec3 worldTarget;
  worldTarget = trans.getMatrix() * localTarget;

  return worldTarget;
}

//----------------------------------------------------------------------------------------------------------------------------
/// Pursuit - uses seek to pursue a moving vehicle,
///           but uses heading so it can calculate where the vehicle will be in the future and aim for that point
//----------------------------------------------------------------------------------------------------------------------------
ngl::Vec3 SteeringBehaviour::Pursuit(const Vehicle *agent)
{
  ngl::Vec3 toAgent = agent->getPos() - m_vehicle->getPos();
  double relativeHeading = m_vehicle->getHeading().dot(agent->getHeading());

  if ((toAgent.dot(m_vehicle->getHeading().dot(agent->getHeading()))) && (relativeHeading < -0.95))
  {
    return Seek(agent->getPos());
  }

  double lookAheadTime = toAgent.length() / (m_vehicle->getMaxSpeed() + agent->getSpeed());
  return Seek(agent->getPos() + agent->getVelocity() * lookAheadTime);
}

//----------------------------------------------------------------------------------------------------------------------------
/// Evade - uses flee to evade a moving vehicle,
///        but uses heading so it can calculate where the vehicle will be in the future and avoid that point
//----------------------------------------------------------------------------------------------------------------------------
ngl::Vec3 SteeringBehaviour::Evade(const Vehicle *agent)
{
  ngl::Vec3 toAgent = agent->getPos() - m_vehicle->getPos();

  double lookAheadTime = toAgent.length() / (m_vehicle->getMaxSpeed() + agent->getSpeed());

  ngl::Vec3 agentPos = agent->getPos() + (agent->getVelocity() * lookAheadTime);

  return Flee(agentPos);
}

//----------------------------------------------------------------------------------------------------------------------------
/// Separation - finds the average separation force for the vehicles closest neighbours
//----------------------------------------------------------------------------------------------------------------------------
ngl::Vec3 SteeringBehaviour::Separation(std::vector<int> neighbours)
{
  ngl::Vec3 separationForce;
  for (unsigned int i = 0; i < neighbours.size(); i++)
  {
    ngl::Vec3 vectorToNeighbour = m_entityMgr->getEntityFromID(neighbours[i])->getPos() - m_vehicle->getPos();

    if(vectorToNeighbour.lengthSquared() == 0.0f)
    {
      std::cout<<"VectorToNeighbour in separation equals zero, can't normalise"<<std::endl;
    }
    else
    {
      vectorToNeighbour.normalize();
    }

    vectorToNeighbour /= -(vectorToNeighbour.length());
    separationForce += vectorToNeighbour;
  }
  return separationForce;
}

//----------------------------------------------------------------------------------------------------------------------------
/// Alignment - finds the average heading of the surrounding neighbours
//----------------------------------------------------------------------------------------------------------------------------
ngl::Vec3 SteeringBehaviour::Alignment(std::vector<int> neighbours)
{
  if (neighbours.size() > 0)
  {
    ngl::Vec3 averageHeading;
    for (unsigned int i = 0; i < neighbours.size(); i++)
    {
      Vehicle* vehicleNeighbour = dynamic_cast<Vehicle*>(m_entityMgr->getEntityFromID(neighbours[i]));
      if (vehicleNeighbour)
      {
        averageHeading += vehicleNeighbour->getHeading();
      }
    }
    averageHeading /= neighbours.size();
    averageHeading -= m_vehicle->getHeading();
    return averageHeading;
  }
  else
  {
    return ngl::Vec3(0.f, 0.f, 0.f);
  }
}

//----------------------------------------------------------------------------------------------------------------------------
/// Cohesion - finds the average position of the vehicles neighbours and seeks to that point
//----------------------------------------------------------------------------------------------------------------------------
ngl::Vec3 SteeringBehaviour::Cohesion(std::vector<int> neighbours)
{
  if (neighbours.size() > 0)
  {
    ngl::Vec3 averagePosition;
    ngl::Vec3 cohesionForce;
    for (unsigned int i =0; i < neighbours.size(); i++)
    {
      averagePosition += m_entityMgr->getEntityFromID(neighbours[i])->getPos();
    }
    averagePosition /= neighbours.size();
    cohesionForce = Seek(averagePosition);

    return cohesionForce;
  }
  else
  {
    return ngl::Vec3(0.f, 0.f, 0.f);
  }
}

//----------------------------------------------------------------------------------------------------------------------------
/// Squad Cohesion - this is the same as arrive, but is used for squads, or protest cohesion only
///                   this is so the weight can be altered using alternate variables
//----------------------------------------------------------------------------------------------------------------------------
ngl::Vec3 SteeringBehaviour::SquadCohesion(ngl::Vec3 squadPos, int deceleration)
{
  //arrive
  ngl::Vec3 toTarget = squadPos - m_vehicle->getPos();
  double dist = toTarget.length();

  if(dist>0.1)
  {
    double decelerationTweak = 0.01;

    double speed = dist/(deceleration*decelerationTweak);
    //make velocity not exceed maxspeed
    if( speed > m_vehicle->getMaxSpeed())
    {
      speed = m_vehicle->getMaxSpeed();
    }

    ngl::Vec3 desiredVelocity = toTarget * speed/dist;
    return(desiredVelocity - m_vehicle->getVelocity());
  }
  else if(dist <= 0.1 )
  {
    return ngl::Vec3(0,0,0);
  }

  return ngl::Vec3(0,0,0);

}

//----------------------------------------------------------------------------------------------------------------------------
/// Obstacle Avoidance - A detection length is generated based on a minimum length plus the ratio between current speed
///                      and max entity speed. A detection box is created with length of detection length and width of
///                      entity radius multiplied by 2. All other entities that are within the detection box plus their own
///                      radius are compared to find the closest entity. The steering force is calculated from the distance
///                      to the closest other entity in the local x and z direction. This local force is then converted
///                      into world space.
//----------------------------------------------------------------------------------------------------------------------------
ngl::Vec3 SteeringBehaviour::ObstacleAvoidance()
{
  //const std::vector<BaseGameEntity *> &obstacles

  float minDetectionLength = m_vehicle->getBoundingRadius() * 1.5;
  float detectionLength = minDetectionLength + ((m_vehicle->getSpeed() / m_vehicle->getMaxSpeed()) * minDetectionLength);

  BaseGameEntity* closestIntersectingObstacle = NULL;
  double distanceToCIO = 99999.9;
  ngl::Vec3 localPosOfCIO;


  int numberOfObstacles = m_vehicle->getNeighbourObstacleIDs().size();

  for (unsigned int i = 0; i < numberOfObstacles; ++i)
  {
    StaticEntity* currentObstacle = dynamic_cast<StaticEntity*>(m_entityMgr->getEntityFromID(m_vehicle->getNeighbourObstacleIDs()[i]));
    if (currentObstacle)
    {
      ngl::Vec3 vectorToObstacle = currentObstacle->getPos() - m_vehicle->getPos();

      if (vectorToObstacle.length() < detectionLength)
        {
          ngl::Vec3 localPos = worldToLocalSpace(currentObstacle->getPos(), m_vehicle->getPos(), m_vehicle->getHeading(), m_vehicle->getSide());

          // if obstacle is behind vehicle in local space, discard
          if (localPos.m_x >= 0.f)
          {
            double addedRadius = currentObstacle->getBoundingRadius() + m_vehicle->getBoundingRadius();
            if (fabs(localPos.m_z) < addedRadius)
            {
              // intersection of radius and line z = 0
              // x = cx +- sqrt(addedRadius^2 - cz^2) where cx and cz are centre coordinates
              double sqrtPart = sqrt(addedRadius*addedRadius - localPos.m_z*localPos.m_z);
              double intersectX = localPos.m_x - sqrtPart;
              if (intersectX <= 0)
              {
                intersectX = localPos.m_x + sqrtPart;
              }
              if (intersectX < distanceToCIO)
              {
                distanceToCIO = intersectX;
                closestIntersectingObstacle = currentObstacle;
                localPosOfCIO = localPos;
              }
            }
        }
      }
    }
  }

  if (closestIntersectingObstacle)
  {
    double mult = 1.0 + (detectionLength - localPosOfCIO.m_x) / detectionLength;
    ngl::Vec3 avoidanceForce;

    if (localPosOfCIO.m_z >= 0)
    {
      avoidanceForce.m_z = (-closestIntersectingObstacle->getBoundingRadius() + localPosOfCIO.m_z) * mult;
    }
    else if (localPosOfCIO.m_z < 0)
    {
      avoidanceForce.m_z = (closestIntersectingObstacle->getBoundingRadius() + localPosOfCIO.m_z) * mult;
    }

    //breaking force
    const double weightBreaking = 1.f;
    avoidanceForce.m_x = -(detectionLength - localPosOfCIO.m_x) * weightBreaking;

    // local to world space
    ngl::Vec3 headingNormalise;
    ngl::Vec3 worldNormalise = ngl::Vec3(1,0,0);
    headingNormalise = m_vehicle->getHeading();

    if(headingNormalise.lengthSquared() == 0.0f)
    {
      std::cout<<"headingNormalise in obstacle avoidance equals zero, can't normalise"<<std::endl;
    }
    else
    {
      headingNormalise.normalize();
    }
    double dotProduct = worldNormalise.dot(headingNormalise);
    double magnitude = headingNormalise.length()*worldNormalise.length();
    double temp = dotProduct/magnitude;
    double angle = acos(temp);
    if(headingNormalise.m_z > 0)
    {
      angle = 2*M_PI - angle;
    }
    ngl::Transformation trans;
    trans.setRotation(0, (-angle * 180)/M_PI, 0);
    ngl::Vec3 worldAvoidanceForce;
    worldAvoidanceForce = trans.getMatrix() * avoidanceForce;


    return worldAvoidanceForce;
  }
  return ngl::Vec3(0.f, 0.f, 0.f);
}

//----------------------------------------------------------------------------------------------------------------------------
/// Wall avoidance - Three feelers are created from the local origin of the entity with forward direction and one 45 degrees
///                  either side. Each of the feelers compare against the walls of the current cell and and check for
///                  intersections. Whichever feeler has the closest intersection creates a force in the direction of the
///                  intersected wall normal, with strength based upon the amount of overlap with the wall.
//----------------------------------------------------------------------------------------------------------------------------
ngl::Vec3 SteeringBehaviour::WallAvoidance()
{
  double feelerLength = m_vehicle->getDetectionRadius();
  std::vector<ngl::Vec3> feelers;
  ngl::Vec3 feelerFront = m_vehicle->getPos() + (feelerLength * m_vehicle->getHeading());
  feelers.push_back(feelerFront);
  ngl::Transformation trans;
  trans.setRotation(0.f, 45.f, 0.f);
  ngl::Vec3 feelerLeft = m_vehicle->getPos() + (feelerLength * (m_vehicle->getHeading() * trans.getMatrix()));
  feelers.push_back(feelerLeft);
  trans.setRotation(0.f, -45.f, 0.f);
  ngl::Vec3 feelerRight = m_vehicle->getPos() + (feelerLength * (m_vehicle->getHeading() * trans.getMatrix()));
  feelers.push_back(feelerRight);

  ngl::Vec3 currentIntersectPoint;
  double distToCurrentIntersect = 0.0;
  ngl::Vec3 closestIntersectPoint;
  double distToClosestIntersect = 99999.9;
  ngl::Vec3 currentClosestWallNormal;
  bool feelerTouch = false;

  ngl::Vec3 wallAvoidanceForce = ngl::Vec3(0.f, 0.f, 0.f);
  int numberOfFeelers = feelers.size();
  for (int i = 0; i < numberOfFeelers; ++i)
  {
    // need neighbour walls
    for (int j= 0; j < m_vehicle->getCurrentCell().getWalls().size(); ++j)
    {
      ngl::Vec3 wallStart = m_vehicle->getCurrentCell().getWalls()[j].start;
      ngl::Vec3 wallEnd = m_vehicle->getCurrentCell().getWalls()[j].end;
      ngl::Vec3 wallLine = wallEnd - wallStart;
      ngl::Vec3 wallNormal = wallLine.cross(ngl::Vec3(0.f, 1.f, 0.f));

      if(wallNormal.lengthSquared() == 0.0f)
      {
        std::cout<<"Wall normal in wall avoidance equals zero, can't normalise"<<std::endl;
      }
      else
      {
        wallNormal.normalize();
      }

      if (lineIntersection2D(m_vehicle->getPos(), feelers[i], wallStart, wallEnd, distToCurrentIntersect, currentIntersectPoint))
      {
        if (distToCurrentIntersect < distToClosestIntersect)
        {
          distToClosestIntersect = distToCurrentIntersect;
          feelerTouch = true;
          closestIntersectPoint = currentIntersectPoint;
          currentClosestWallNormal = wallNormal;
        }
      }

    }
    if (feelerTouch == true)
    {
      ngl::Vec3 overShoot = feelers[i] - closestIntersectPoint;
      ngl::Vec3 reverseOverShoot = currentClosestWallNormal * overShoot.length();
      wallAvoidanceForce = reverseOverShoot;

    }
  }
  return wallAvoidanceForce;
}

//----------------------------------------------------------------------------------------------------------------------------
/// This method converts points from world space to local space
//----------------------------------------------------------------------------------------------------------------------------
ngl::Vec3 SteeringBehaviour::worldToLocalSpace(ngl::Vec3 pointWorldPos, ngl::Vec3 vehiclePos, ngl::Vec3 vehicleHeading, ngl::Vec3 vehicleSide)
{
  if(vehicleHeading.lengthSquared() == 0.0f)
  {
    std::cout<<"Vehicle heading in worldToLocalSpace equals zero, can't normalise"<<std::endl;
  }
  else
  {
    vehicleHeading.normalize();
  }
  if(vehicleSide.lengthSquared() == 0.0f)
  {
    std::cout<<"Vehicle side in worldToLocalSpace equals zero, can't normalise"<<std::endl;
  }
  else
  {
    vehicleSide.normalize();
  }

  double Tx = vehiclePos.dot(vehicleHeading);
  double Tz = vehiclePos.dot(vehicleSide);

  ngl::Mat4 mat;
  mat.m_00 = vehicleHeading.m_x; mat.m_02 = vehicleHeading.m_z; mat.m_03 = -Tx;
  mat.m_20 = vehicleSide.m_x;    mat.m_22 = vehicleSide.m_z;    mat.m_23 = -Tz;
  double tempX = (mat.m_00 * pointWorldPos.m_x) + (mat.m_02 * pointWorldPos.m_z) + mat.m_03;
  double tempZ = (mat.m_20 * pointWorldPos.m_x) + (mat.m_22 * pointWorldPos.m_z) + mat.m_23;

  return ngl::Vec3(tempX, 0, tempZ);
}


//----------------------------------------------------------------------------------------------------------------------------
void SteeringBehaviour::addAllNeighbours(std::vector<int> _neighbours)
{
  for (int i = 0; i < _neighbours.size(); i++)
  {
    m_allNeighbours.push_back(_neighbours[i]);
  }

}


//----------------------------------------------------------------------------------------------------------------------------
void SteeringBehaviour::addFriendlyNeighbours(std::vector<int> _neighbours)
{
  for (unsigned int i = 0; i < _neighbours.size(); i++)
  {
    m_friendlyNeighbours.push_back(_neighbours[i]);
  }
}


//----------------------------------------------------------------------------------------------------------------------------
/// this method uses the equations from http://paulbourke.net/geometry/pointlineplane/ to determin wether two 2D lines
/// intersect. It also calculates the interesection point and the distance to the interesections point.
//----------------------------------------------------------------------------------------------------------------------------
bool SteeringBehaviour::lineIntersection2D(ngl::Vec3 startLineA, ngl::Vec3 endLineA, ngl::Vec3 startLineB, ngl::Vec3 endLineB,
                                           double &o_distToIntersect, ngl::Vec3 &o_intersectPoint)
{
  double rTop = (startLineA.m_z-startLineB.m_z)*(endLineB.m_x-startLineB.m_x)-(startLineA.m_x-startLineB.m_x)*(endLineB.m_z-startLineB.m_z);
  double rBot = (endLineA.m_x-startLineA.m_x)*(endLineB.m_z-startLineB.m_z)-(endLineA.m_z-startLineA.m_z)*(endLineB.m_x-startLineB.m_x);

  double sTop = (startLineA.m_z-startLineB.m_z)*(endLineA.m_x-startLineA.m_x)-(startLineA.m_x-startLineB.m_x)*(endLineA.m_z-startLineA.m_z);
  double sBot = (endLineA.m_x-startLineA.m_x)*(endLineB.m_z-startLineB.m_z)-(endLineA.m_z-startLineA.m_z)*(endLineB.m_x-startLineB.m_x);

  if ((rBot == 0) || (sBot == 0))
  {
    return false;
  }

  double r = rTop/rBot;
  double s = sTop/sBot;

  if ((r > 0.0) && (r < 1.0) && (s > 0.0) && (s < 1.0))
  {
    ngl::Vec3 lineA = endLineA - startLineA;
    o_distToIntersect = lineA.length() * r;
    o_intersectPoint = startLineA + (r * (endLineA - startLineA));
    return true;
  }
  else
  {
    //dist = 0.0;
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------
/// this object overlap avoidance method stops objects from overalpping by comparing the distance between them against
/// the sum of their radii. If the distance is less than their radii sum then they two entities are moved apart in the
/// direction of the vector between them by a distance equal to the amount of overlap#
//----------------------------------------------------------------------------------------------------------------------------
void SteeringBehaviour::ObjectOverlapAvoidance()
{


  int numberOfNeighbours = m_allNeighbours.size();
  for (unsigned int i = 0; i < numberOfNeighbours; i++)
  {
    Vehicle* curEntity = dynamic_cast<Vehicle*>(m_entityMgr->getEntityFromID(m_allNeighbours[i]));
    if (curEntity)
    {
      //make sure we don't check against ourselves
      Vehicle* entity = dynamic_cast<Vehicle*>(m_entityMgr->getEntityFromID(m_vehicle->getID()));
      if (curEntity == entity) continue;

      ngl::Vec3 vecToEntity = m_vehicle->getPos() - curEntity->getPos();
      double distFromEachOther = vecToEntity.length();

      double amountOfOverLap = (m_vehicle->getBoundingRadius() + curEntity->getBoundingRadius()) - distFromEachOther;

      if (amountOfOverLap >= 0)
      {
        m_vehicle->setPos(m_vehicle->getPos() + (vecToEntity/distFromEachOther) * amountOfOverLap);
      }
     }
  }

  for (unsigned int i = 0; i < m_vehicle->getNeighbourObstacleIDs().size(); i++)
  {
    StaticEntity* curEntity = dynamic_cast<StaticEntity*>(m_entityMgr->getEntityFromID(m_vehicle->getNeighbourObstacleIDs()[i]));
    if (curEntity)
    {
      ngl::Vec3 vecToEntity = m_vehicle->getPos() - curEntity->getPos();
      double distFromEachOther = vecToEntity.length();

      double amountOfOverLap = (m_vehicle->getBoundingRadius() + curEntity->getBoundingRadius()) - distFromEachOther;

      if (amountOfOverLap >= 0)
      {
        m_vehicle->setPos(m_vehicle->getPos() + (vecToEntity/distFromEachOther) * amountOfOverLap);
      }
    }
  }

}

//----------------------------------------------------------------------------------------------------------------------------
/// wall overlap avoidance checks the entity against the walls in the current cell. If the distance between the wall location
/// and the entity centre is less than the radius of the entity then the entity is moved in the direction of the wall normal
/// by the amount of overlap with the wall
//----------------------------------------------------------------------------------------------------------------------------
void SteeringBehaviour::WallOverlapAvoidance()
{
  for (int j= 0; j < m_vehicle->getCurrentCell().getWallsInCell().size(); ++j)
  {
    ngl::Vec3 wallStart = m_vehicle->getCurrentCell().getWallsInCell()[j].start;
    ngl::Vec3 wallEnd = m_vehicle->getCurrentCell().getWallsInCell()[j].end;
    ngl::Vec3 wallLine = wallEnd - wallStart;
    ngl::Vec3 wallNormal = wallLine.cross(ngl::Vec3(0.f, 1.f, 0.f));

    if(wallNormal.lengthSquared() == 0.0f)
    {
      std::cout<<"wall normal in wall overlap avoidance equals zero, can't normalise"<<std::endl;
    }
    else
    {
      wallNormal.normalize();
    }

    if (wallLine.m_x != 0.f)
    {
      // upper or lower wall
      float zDistToWall = fabs(wallStart.m_z - m_vehicle->getPos().m_z);

      float amountOfOverLap = m_vehicle->getBoundingRadius() - zDistToWall;
      if (amountOfOverLap >= 0)
      {
        m_vehicle->setPos(m_vehicle->getPos() + (wallNormal * amountOfOverLap));
      }
    }
    else if (wallLine.m_z != 0.f)
    {
      // left or right wall
      float xDistToWall = fabs(wallStart.m_x - m_vehicle->getPos().m_x);

      float amountOfOverLap = m_vehicle->getBoundingRadius() - xDistToWall;
      if (amountOfOverLap >= 0)
      {
        m_vehicle->setPos(m_vehicle->getPos() + (wallNormal * amountOfOverLap));
      }
    }

  }
}
