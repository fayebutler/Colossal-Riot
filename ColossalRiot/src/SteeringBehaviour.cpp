#include "SteeringBehaviour.h"
#include "Vehicle.h"
#include <math.h>
#include "GameWorld.h"
#include <cstdlib>
#include <ngl/Random.h>
#include <assert.h>
#include "Cell.h"

SteeringBehaviour::SteeringBehaviour(Vehicle* agent):
    m_vehicle(agent),
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
//    m_weightFollowPath(1.0),
    m_weightPursuit(1.0),
    m_weightEvade(1.0)
//    m_weightInterpose(1.0),
//    m_weightHide(1.0),
//    m_weightFlock(1.0),
//    m_weightOffsetPursuit(1.0)
{
  m_entityMgr = new EntityManager();

//    double theta = (rand()/(RAND_MAX+1.0)) * (M_PI * 2);
    double theta = (float)rand()/RAND_MAX * (M_PI * 2);
    m_wanderTarget = ngl::Vec3(m_wanderRadius * cos(theta), 0, m_wanderRadius * sin(theta));

    m_targetAgent = NULL;
}

SteeringBehaviour::~SteeringBehaviour()
{
    //delete m_entityMgr;
}



ngl::Vec3 SteeringBehaviour::calculate()
{
    //simple weight truncated sum

 //   force += Seek(ngl::Vec3(0,0,0)) * m_weightSeek;
    m_steeringForce += Wander() * m_weightWander;
    //force += Flee(ngl::Vec3(10,10,10)) *m_weightFlee;

    //use weight truncated running sum prioritisation
    //truncate force to maxforce
    if(m_steeringForce.lengthSquared() >= (m_vehicle->getMaxForce() * m_vehicle->getMaxForce()))
    {
        if(m_steeringForce.lengthSquared() == 0.0f)
        {
            std::cout<<"Steering Force in calculate equals zero, can't normalise"<<std::endl;
        }
        else
        {
            m_steeringForce.normalize();
        }
        m_steeringForce = m_steeringForce * m_vehicle->getMaxForce();
    }
    return m_steeringForce;
}


ngl::Vec3 SteeringBehaviour::calculateWeightedSum()
{
    m_steeringForce = ngl::Vec3(0.f, 0.f, 0.f);
    if(on(seek)) //need to create crosshair in gameworld
    {
        m_steeringForce += Seek(m_vehicle->getCrosshair()) * m_weightSeek;
    }
    if(on(flee))
    {
        m_steeringForce += Flee(ngl::Vec3(0,0,0)) * m_weightFlee;
    }
    if(on(arrive))
    {
        m_steeringForce += Arrive(m_vehicle->getCrosshair(), m_deceleration) * m_weightArrive;
    }
    if(on(pursuit))
    {
        assert(m_targetAgent && "pursuit target not assigned");
        m_steeringForce += Pursuit(m_targetAgent) * m_weightPursuit;
    }
    if(on(wander))
    {
        m_steeringForce += Wander() * m_weightWander;
    }
    if(on(evade))
    {
        assert(m_targetAgent && "evade target not assigned");
        m_steeringForce += Evade(m_targetAgent) * m_weightEvade;
    }

    if(on(obstacle_avoidance))
    {
        m_steeringForce += ObstacleAvoidance() * m_weightObstacleAvoidance;

    }


   //truncate steering force to max force
    if(m_steeringForce.lengthSquared() > (m_vehicle->getMaxForce() * m_vehicle->getMaxForce()))
    {
        if(m_steeringForce.lengthSquared() == 0.0f)
        {
            std::cout<<"Steering Force in calculateWeightedSum equals zero, can't normalise"<<std::endl;
        }
        else
        {
            m_steeringForce.normalize();
        }
        m_steeringForce = m_steeringForce * m_vehicle->getMaxForce();
    }

    return m_steeringForce;

}


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
        if (m_targetAgent == NULL)
        {
           //std::cout<<"no target agent assigned for pursuit"<<std::endl;
        }
        else
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
        if (m_targetAgent == NULL)
        {
            //std::cout<<"no target agent assigned for evade"<<std::endl;
        }
        else
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
        force = SquadCohesion(m_vehicle->getSquadCrosshair()) * m_weightSquadCohesion;
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

    //std::cout<<"finish calculate"<<std::endl;
    return m_steeringForce;


}

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


double SteeringBehaviour::forwardComponent()
{
    return m_vehicle->getHeading().dot(m_steeringForce);
}

double SteeringBehaviour::sideComponent()
{
    return m_vehicle->getSide().dot(m_steeringForce);
}

//behaviour type functions

ngl::Vec3 SteeringBehaviour::Seek(ngl::Vec3 TargetPos)
{
    ngl::Vec3 desiredVelocity = ngl::Vec3(TargetPos - m_vehicle->getPos());

//    assert(desiredVelocity.length() != 0 && "desiredVel in seek EQUALS ZERO ");

    if(desiredVelocity.lengthSquared() == 0.0f)
    {
        std::cout<<" Desired Velocity in Seek equals zero, can't normalize"<<std::endl;
        std::cout<<"Desired Velocity "<<desiredVelocity.m_x<<" "<<desiredVelocity.m_y<<" "<<desiredVelocity.m_z<<std::endl;
        return desiredVelocity - m_vehicle->getVelocity();
    }
    else
    {
        desiredVelocity.normalize();
        desiredVelocity = desiredVelocity * m_vehicle->getMaxSpeed();
        return (desiredVelocity - m_vehicle->getVelocity());
    }

}

ngl::Vec3 SteeringBehaviour::Flee(ngl::Vec3 TargetPos)
{

    ngl::Vec3 desiredVelocity = ngl::Vec3(m_vehicle->getPos() - TargetPos);

    if(desiredVelocity.lengthSquared() == 0.0f)
    {
        std::cout<<"Desired Velocity in Flee equals zero, can't normalise"<<std::endl;
        return desiredVelocity - m_vehicle->getVelocity();
    }
    else
    {
        desiredVelocity.normalize();
        desiredVelocity = desiredVelocity * m_vehicle->getMaxSpeed();
        return(desiredVelocity - m_vehicle->getVelocity());
    }

}

ngl::Vec3 SteeringBehaviour::Arrive(ngl::Vec3 TargetPos, int deceleration)
{
//    std::cout<<"CALLING ARRIVE"<<std::endl;
    ngl::Vec3 toTarget = TargetPos - m_vehicle->getPos();
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
//        m_vehicle->setHeading(ngl::Vec3(-1,0,0));
        //ArriveOff();
        return ngl::Vec3(0,0,0);
    }

    return ngl::Vec3(0,0,0);
}


ngl::Vec3 SteeringBehaviour::Wander()
{

  double jitterTimeSlice = m_wanderJitter * m_vehicle->TimeElapsed() ;

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

  if(worldTarget.lengthSquared() == 0.0f)
  {
      std::cout<<"World Target in wander equals zero, can't normalise"<<std::endl;
  }
  else
  {
    worldTarget.normalize();
  }

  worldTarget = worldTarget * m_vehicle->getMaxSpeed();

  return worldTarget;

}

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

ngl::Vec3 SteeringBehaviour::SquadCohesion(ngl::Vec3 SquadPos)
{
    ngl::Vec3 desiredVelocity = ngl::Vec3(SquadPos - m_vehicle->getPos());

//    assert(desiredVelocity.length() != 0 && "desiredVel in seek EQUALS ZERO ");

    if(desiredVelocity.lengthSquared() == 0.0f)
    {
        std::cout<<" Desired Velocity in Squad Pos equals zero, can't normalize"<<std::endl;
        std::cout<<"Desired Velocity "<<desiredVelocity.m_x<<" "<<desiredVelocity.m_y<<" "<<desiredVelocity.m_z<<std::endl;
        return desiredVelocity - m_vehicle->getVelocity();
    }
    else
    {
        desiredVelocity.normalize();
        desiredVelocity = desiredVelocity * m_vehicle->getMaxSpeed();
        return (desiredVelocity - m_vehicle->getVelocity());
    }

}


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

ngl::Vec3 SteeringBehaviour::Evade(const Vehicle *agent)
{


    ngl::Vec3 toAgent = agent->getPos() - m_vehicle->getPos();

//    if only want to conside pursuers within range
//    double threatRange = 100.0;
//    if(toAgent.lengthSquared() > threatRange * threatRange) return ngl::Vec3();

    double lookAheadTime = toAgent.length() / (m_vehicle->getMaxSpeed() + agent->getSpeed());

    ngl::Vec3 agentPos = agent->getPos() + (agent->getVelocity() * lookAheadTime);

    return Flee(agentPos);
}

ngl::Vec3 SteeringBehaviour::ObstacleAvoidance()
{
  //const std::vector<BaseGameEntity *> &obstacles

  // not sure if I should use radius for detectionLength, or create a new minimumLength variable
  float minDetectionLength = m_vehicle->getBoundingRadius() * 1.5;
  float detectionLength = minDetectionLength + ((m_vehicle->getSpeed() / m_vehicle->getMaxSpeed()) * minDetectionLength);

  // this needs to be a get agents within detection length as a detection radius
  // but for now we will just get all agents

  BaseGameEntity* closestIntersectingObstacle = NULL;
  // any way to get maximum double value?
  double distanceToCIO = 99999.9;
  ngl::Vec3 localPosOfCIO;


  int numberOfRioters = m_vehicle->getNeighbourRioterIDs().size();
  for (unsigned int i = 0; i < numberOfRioters; i++)
  {
    Rioter* currentRioter = dynamic_cast<Rioter*>(m_entityMgr->getEntityFromID(m_vehicle->getNeighbourRioterIDs()[i]));
    if (currentRioter)
    {
      if (currentRioter->getID() != m_vehicle->getID())
      {
        ngl::Vec3 vectorToObstacle = currentRioter->getPos() - m_vehicle->getPos();

        if (vectorToObstacle.length() < detectionLength)
        {

        ngl::Vec3 localPos = worldToLocalSpace(currentRioter->getPos(), m_vehicle->getPos(), m_vehicle->getHeading(), m_vehicle->getSide());

          // if obstacle is behind vehicle in local space, discard
          if (localPos.m_x >= 0.f)
          {
            double addedRadius = currentRioter->getBoundingRadius() + m_vehicle->getBoundingRadius();
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
                closestIntersectingObstacle = currentRioter;
                localPosOfCIO = localPos;
              }
            }
          }
        }
      }
    }
  }

  int numberOfPolice = m_vehicle->getNeighbourPoliceIDs().size();

  for (unsigned int i = 0; i < numberOfPolice; ++i)
  {
    Police* currentPolice = dynamic_cast<Police*>(m_entityMgr->getEntityFromID(m_vehicle->getNeighbourPoliceIDs()[i]));
    if (currentPolice)
    {
      if (currentPolice->getID() != m_vehicle->getID())
      {
        ngl::Vec3 vectorToObstacle = currentPolice->getPos() - m_vehicle->getPos();

        if (vectorToObstacle.length() < detectionLength)
        {

          ngl::Vec3 localPos = worldToLocalSpace(currentPolice->getPos(), m_vehicle->getPos(), m_vehicle->getHeading(), m_vehicle->getSide());


          // if obstacle is behind vehicle in local space, discard
          if (localPos.m_x >= 0.f)
          {
            double addedRadius = currentPolice->getBoundingRadius() + m_vehicle->getBoundingRadius();
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
                closestIntersectingObstacle = currentPolice;
                localPosOfCIO = localPos;
              }
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

    if(worldAvoidanceForce.lengthSquared() == 0.0f)
    {
        std::cout<<"worldAvoidanceForce in obstacleAvoidance equals zero, can't normalise"<<std::endl;
    }
    else
    {
      worldAvoidanceForce.normalize();
    }
    worldAvoidanceForce *= m_vehicle->getMaxSpeed();

    return worldAvoidanceForce;
   // worldAvoidanceForce.normalize();
    //worldAvoidanceForce = worldAvoidanceForce * m_vehicle->getMaxSpeed();
  }

  return ngl::Vec3(0.f, 0.f, 0.f);

}

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

  for (int i = 0; i < feelers.size(); ++i)
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

void SteeringBehaviour::addAllNeighbours(std::vector<int> neighbours)
{
  for (int i = 0; i < neighbours.size(); i++)
  {

    m_allNeighbours.push_back(neighbours[i]);
  }

}

void SteeringBehaviour::addFriendlyNeighbours(std::vector<int> neighbours)
{
  for (unsigned int i = 0; i < neighbours.size(); i++)
  {
    m_friendlyNeighbours.push_back(neighbours[i]);
  }
}

bool SteeringBehaviour::lineIntersection2D(ngl::Vec3 startLineA, ngl::Vec3 endLineA, ngl::Vec3 startLineB, ngl::Vec3 endLineB, double &distToIntersect, ngl::Vec3 &intersectPoint)
{
  // equation from http://paulbourke.net/geometry/pointlineplane/


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
    distToIntersect = lineA.length() * r;
    intersectPoint = startLineA + (r * (endLineA - startLineA));
    return true;
  }
  else
  {
    //dist = 0.0;
    return false;
  }
}



void SteeringBehaviour::ObjectOverlapAvoidance()
{
    for (unsigned int i = 0; i < m_allNeighbours.size(); i++)

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

}

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
        //m_vehicle->setHeading(m_vehicle->getHeading() * wallNormal);

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
        //m_vehicle->setHeading(m_vehicle->getHeading() * wallNormal);
      }
    }

  }
}
