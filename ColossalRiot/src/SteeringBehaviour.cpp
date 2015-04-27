#include "SteeringBehaviour.h"
#include "Vehicle.h"
#include <math.h>
#include "GameWorld.h"
#include <cstdlib>
#include <ngl/Random.h>
#include <assert.h>

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
//    m_weightWallAvoidance(1.0),
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
}



ngl::Vec3 SteeringBehaviour::calculate()
{
    //simple weight truncated sum

 //   force += Seek(ngl::Vec3(0,0,0)) * m_weightSeek;
    m_steeringForce += Wander() * m_weightWander;
    //force += Flee(ngl::Vec3(10,10,10)) *m_weightFlee;

    //use weight truncated running sum prioritisation
    //truncate force to maxforce
    if(m_steeringForce.length() >= m_vehicle->getMaxForce())
    {
        m_steeringForce.normalize();
        m_steeringForce = m_steeringForce * m_vehicle->getMaxForce();
    }
    return m_steeringForce;
}


ngl::Vec3 SteeringBehaviour::calculateWeightedSum()
{
    m_steeringForce = 0;

    m_steeringForce = 0;
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
    if(m_steeringForce.length() > m_vehicle->getMaxForce())
    {
        m_steeringForce.normalize();
        m_steeringForce = m_steeringForce * m_vehicle->getMaxForce();
    }

    return m_steeringForce;

}


ngl::Vec3 SteeringBehaviour::calculatePrioritizedSum()
{
    m_steeringForce = 0;

    ngl::Vec3 force;
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
      force = Separation(m_neighbours) * m_weightSeparation;
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
      force = Cohesion(m_neighbours) * m_weightCohesion;
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
      force = Alignment(m_neighbours) * m_weightAlignment;
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
        assert(m_targetAgent && "pursuit target not assigned");
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
    if(on(evade))
    {
        assert(m_targetAgent && "evade target not assigned");
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

bool SteeringBehaviour::accumulateForce(ngl::Vec3 currentTotal, ngl::Vec3 force)
{
    double magnitude = currentTotal.length();

    double forceLeft =  m_vehicle->getMaxForce() - magnitude;

    if(forceLeft < 0.0)
    {
        return false;
    }
    else if(force.length() + currentTotal.length() > m_vehicle->getMaxForce())
    {
        return false;
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
    desiredVelocity.normalize();
    desiredVelocity = desiredVelocity * m_vehicle->getMaxSpeed();
    return (desiredVelocity - m_vehicle->getVelocity());
}

ngl::Vec3 SteeringBehaviour::Flee(ngl::Vec3 TargetPos)
{

    ngl::Vec3 desiredVelocity = ngl::Vec3(m_vehicle->getPos() - TargetPos);
    desiredVelocity.normalize();
    desiredVelocity = desiredVelocity * m_vehicle->getMaxSpeed();
    return(desiredVelocity - m_vehicle->getVelocity());
}

ngl::Vec3 SteeringBehaviour::Arrive(ngl::Vec3 TargetPos, int deceleration)
{

    ngl::Vec3 toTarget = TargetPos - m_vehicle->getPos();
    double dist = toTarget.length();

    if(dist>0)
    {
        double decelerationTweak = 0.3;

        double speed = dist/(deceleration*decelerationTweak);
        //make velocity not exceed maxspeed
        if( speed > m_vehicle->getMaxSpeed())
        {
            speed = m_vehicle->getMaxSpeed();
        }

        ngl::Vec3 desiredVelocity = toTarget * speed/dist;
        return(desiredVelocity - m_vehicle->getVelocity());
    }

    return ngl::Vec3(0,0,0);
}


ngl::Vec3 SteeringBehaviour::Wander()
{

  double jitterTimeSlice = m_wanderJitter * m_vehicle->TimeElapsed() ;

  float randomClampedX = -1+2*((float)rand())/RAND_MAX;
  float randomClampedZ = -1+2*((float)rand())/RAND_MAX;

  m_wanderTarget += ngl::Vec3(randomClampedX * jitterTimeSlice ,0, randomClampedZ * jitterTimeSlice);
  m_wanderTarget.normalize();
  m_wanderTarget *= m_wanderRadius;
  ngl::Vec3 localTarget = m_wanderTarget + ngl::Vec3(m_wanderDistance,0,0);

  // local to world space
  ngl::Vec3 headingNormalise;
  ngl::Vec3 worldNormalise = ngl::Vec3(1,0,0);
  headingNormalise = m_vehicle->getHeading();
  headingNormalise.normalize();
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

  worldTarget.normalize();
  worldTarget = worldTarget * m_vehicle->getMaxSpeed();

  return worldTarget;

}

ngl::Vec3 SteeringBehaviour::Separation(std::vector<int> neighbours)
{
  ngl::Vec3 separationForce;
  for (unsigned int i = 0; i < neighbours.size(); i++)
  {
    ngl::Vec3 vectorToNeighbour = m_entityMgr->getEntityFromID(neighbours[i])->getPos() - m_vehicle->getPos();
    vectorToNeighbour.normalize();
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

  for (unsigned int i = 0; i < m_vehicle->World()->getRioters().size(); ++i)
  {
    Rioter* currentRioter = m_vehicle->World()->getRioters()[i];

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
  for (unsigned int i = 0; i < m_vehicle->World()->getPolice().size(); ++i)
  {
    Police* currentPolice = m_vehicle->World()->getPolice()[i];
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
    headingNormalise.normalize();
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

    worldAvoidanceForce.normalize();
    worldAvoidanceForce *= m_vehicle->getMaxSpeed();

    return worldAvoidanceForce;
    worldAvoidanceForce.normalize();
    worldAvoidanceForce = worldAvoidanceForce * m_vehicle->getMaxSpeed();
  }
  return ngl::Vec3(0.f, 0.f, 0.f);


}

ngl::Vec3 SteeringBehaviour::wallAvoidance()
{
  m_vehicle->getCurrentCell()
}

ngl::Vec3 SteeringBehaviour::worldToLocalSpace(ngl::Vec3 pointWorldPos, ngl::Vec3 vehiclePos, ngl::Vec3 vehicleHeading, ngl::Vec3 vehicleSide)
{
  vehicleHeading.normalize();
  vehicleSide.normalize();

  double Tx = vehiclePos.dot(vehicleHeading);
  double Tz = vehiclePos.dot(vehicleSide);

  ngl::Mat4 mat;
  mat.m_00 = vehicleHeading.m_x; mat.m_02 = vehicleHeading.m_z; mat.m_03 = -Tx;
  mat.m_20 = vehicleSide.m_x;    mat.m_22 = vehicleSide.m_z;    mat.m_23 = -Tz;
  double tempX = (mat.m_00 * pointWorldPos.m_x) + (mat.m_02 * pointWorldPos.m_z) + mat.m_03;
  double tempZ = (mat.m_20 * pointWorldPos.m_x) + (mat.m_22 * pointWorldPos.m_z) + mat.m_23;

  return ngl::Vec3(tempX, 0, tempZ);
}

ngl::Vec3 SteeringBehaviour::localToWorldSpace(ngl::Vec3 pointLocalPos, ngl::Vec3 vehiclePos, ngl::Vec3 vehicleHeading)
{
//  vehicleHeading.normalize();
//  ngl::Vec3 worldNormalise = ngl::Vec3(1,0,0);

//  double dotProduct = worldNormalise.dot(vehicleHeading);
//  double magnitude = vehicleHeading.length()*worldNormalise.length();
//  double temp = dotProduct/magnitude;
//  double angle = acos(temp);
//  if(vehicleHeading.m_z > 0)
//  {
//    angle = 2*M_PI - angle;
//  }
//  ngl::Transformation trans;
//  trans.setRotation(0, (-angle * 180)/M_PI, 0);
//  ngl::Vec3 worldPos;
//  worldPos = trans.getMatrix() * pointLocalPos;

//  return worldPos;
}

void SteeringBehaviour::addNeighbours(std::vector<int> neighbours)
{
  for (int i = 0; i < neighbours.size(); i++)
  {
    m_neighbours.push_back(neighbours[i]);
  }
}

