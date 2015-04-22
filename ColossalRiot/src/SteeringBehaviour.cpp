#include "SteeringBehaviour.h"
#include "Vehicle.h"
#include <math.h>
#include "GameWorld.h"
#include <cstdlib>
#include <ngl/Random.h>
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
//    m_weightCohesion(1.0),
//    m_weightSeparation(1.0),
//    m_weightAlignment(1.0),
//    m_weightObstacleAvoidance(1.0),
//    m_weightWallAvoidance(1.0),
//    m_weightFollowPath(1.0),
    m_weightPursuit(1.0),
    m_weightEvade(1.0)
//    m_weightInterpose(1.0),
//    m_weightHide(1.0),
//    m_weightFlock(1.0),
//    m_weightOffsetPursuit(1.0)
{
//    double theta = (rand()/(RAND_MAX+1.0)) * (M_PI * 2);
    double theta = (float)rand()/RAND_MAX * (M_PI * 2);
    m_wanderTarget = ngl::Vec3(m_wanderRadius * cos(theta), 0, m_wanderRadius * sin(theta));
    m_wanderTargetOriginal = m_wanderTarget;
    std::cout<<"wanderTarget "<<m_wanderTarget[0]<<", "<<m_wanderTarget[1]<<", "<<m_wanderTarget[2]<<std::endl;
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

//    if(on(seek)) //need to create crosshair in gameworld
//    {
//        m_steeringForce += Seek(ngl::Vec3(0,0,0)) * m_weightSeek;
//    }
//    if(on(flee))
//    {
//        m_steeringForce += Flee(ngl::Vec3(0,0,0)) * m_weightFlee;
//    }
//    if(on(arrive))
//    {
//        m_steeringForce += Arrive(m_vehicle->World()->Crosshair(), m_deceleration) * m_weightArrive;
//    }
//    if(on(pursuit))
//    {
//        m_steeringForce += Pursuit(m_targetAgent) * m_weightPursuit;
//    }
    if(on(wander))
    {
        m_steeringForce = Wander() * m_weightWander;
    }
//    if(on(evade))
//    {
//        m_steeringForce += Evade(m_targetAgent) * m_weightEvade;
//    }

    std::cout<<"Steering Force = "<<m_steeringForce.m_x<<"  "<<m_steeringForce.m_y<<"  "<<m_steeringForce.m_z<<std::endl;

   //truncate steering force to max force
    if(m_steeringForce.length() > m_vehicle->getMaxForce())
    {
        m_steeringForce.normalize();
        std::cout<<"Steering Force Normalized = "<<m_steeringForce.m_x<<"  "<<m_steeringForce.m_y<<"  "<<m_steeringForce.m_z<<std::endl;
//        m_steeringForce = m_steeringForce * m_vehicle->getMaxForce();
    }

    return m_steeringForce;

}

double SteeringBehaviour::forwardComponent()
{
    return m_vehicle->getHeading().dot(m_steeringForce);
}

double SteeringBehaviour::sideComponent()
{
    return m_vehicle->getSide().dot(m_steeringForce);
}

//bejaviour type functions

ngl::Vec3 SteeringBehaviour::Seek(ngl::Vec3 TargetPos)
{
    std::cout<<"SteeringBehaviour::Seek"<<std::endl;
    ngl::Vec3 desiredVelocity = ngl::Vec3(TargetPos - m_vehicle->getPos());
    desiredVelocity.normalize();
    desiredVelocity = desiredVelocity * m_vehicle->getMaxSpeed();

    return (desiredVelocity - m_vehicle->getVelocity());
}

ngl::Vec3 SteeringBehaviour::Flee(ngl::Vec3 TargetPos)
{
    std::cout<<"SteeringBehaviour::Flee"<<std::endl;
    ngl::Vec3 desiredVelocity = ngl::Vec3(m_vehicle->getPos() - TargetPos);
    desiredVelocity.normalize();
    desiredVelocity = desiredVelocity * m_vehicle->getMaxSpeed();
    return(desiredVelocity - m_vehicle->getVelocity());
}

ngl::Vec3 SteeringBehaviour::Arrive(ngl::Vec3 TargetPos, int deceleration)
{
    std::cout<<"SteeringBehaviour::Arrive"<<std::endl;
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
  std::cout<<"SteeringBehaviour::Wander"<<std::endl;
  std::cout<<"wanderTargetOriginal "<<m_wanderTargetOriginal[0]<<", "<<m_wanderTargetOriginal[1]<<", "<<m_wanderTargetOriginal[2]<<std::endl;

  double jitterTimeSlice = m_wanderJitter * m_vehicle->TimeElapsed() ;
  std::cout<<"jitterTimeSlice "<<jitterTimeSlice<<std::endl;

  float randomClampedX = -1+2*((float)rand())/RAND_MAX;
  float randomClampedZ = -1+2*((float)rand())/RAND_MAX;
  std::cout<<"randomClampedX "<<randomClampedX<<std::endl;
  std::cout<<"randomClampedZ "<<randomClampedZ<<std::endl;

  std::cout<<"wanderTarget before "<<m_wanderTarget[0]<<", "<<m_wanderTarget[1]<<", "<<m_wanderTarget[2]<<std::endl;
  m_wanderTarget += ngl::Vec3(randomClampedX * jitterTimeSlice ,0, randomClampedZ * jitterTimeSlice);

  std::cout<<"wanderTarget after  "<<m_wanderTarget[0]<<", "<<m_wanderTarget[1]<<", "<<m_wanderTarget[2]<<std::endl;
  m_wanderTarget.normalize();

  m_wanderTarget *= m_wanderRadius;
  std::cout<<"wanderTarget radius "<<m_wanderTarget[0]<<", "<<m_wanderTarget[1]<<", "<<m_wanderTarget[2]<<std::endl;

  //m_wanderTarget = ngl::Vec3(0.f, 0.f, 2.f);
  ngl::Vec3 localTarget = m_wanderTarget + ngl::Vec3(m_wanderDistance,0,0);

  std::cout<<"localTarget "<<localTarget[0]<<", "<<localTarget[1]<<", "<<localTarget[2]<<std::endl;

  ngl::Vec3 headingNormalise;
  ngl::Vec3 worldNormalise = ngl::Vec3(1,0,0);
  headingNormalise = m_vehicle->getHeading();
  headingNormalise.normalize();
  std::cout<<"headingNormalise "<<headingNormalise[0]<<", "<<headingNormalise[1]<<", "<<headingNormalise[2]<<std::endl;
  double dotProduct = worldNormalise.dot(headingNormalise);

  std::cout<<"dotProduct "<<dotProduct<<std::endl;
  double magnitude = headingNormalise.length()*worldNormalise.length();
  std::cout<<"magnitude "<<magnitude<<std::endl;
  double temp = dotProduct/magnitude;
  std::cout<<"temp "<<temp<<std::endl;
  double angle = acos(temp);

  if(headingNormalise.m_z > 0)
  {
    angle = 2*M_PI - angle;
  }

  std::cout<<"angle "<<((angle * 180)/M_PI)<<std::endl;

  ngl::Transformation trans;
  trans.setRotation(0, (-angle * 180)/M_PI, 0);
  ngl::Vec3 worldTarget;
  worldTarget = trans.getMatrix() * localTarget;
  worldTarget += m_vehicle->getPos();

  std::cout<<"worldTarget "<<worldTarget[0]<<", "<<worldTarget[1]<<", "<<worldTarget[2]<<std::endl;
  std::cout<<"localTarget "<<localTarget[0]<<", "<<localTarget[1]<<", "<<localTarget[2]<<std::endl;

  //worldTarget = ngl::Vec3(3.f, 0.f, 1.f);
  m_worldWanderTarget = worldTarget;
  m_localWanderTarget = localTarget;
  return worldTarget;
}



ngl::Vec3 SteeringBehaviour::Pursuit(const Vehicle *agent)
{
  std::cout<<"SteeringBehaviour::Persuit"<<std::endl;
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
  std::cout<<"SteeringBehaviour::Evade"<<std::endl;
    ngl::Vec3 toAgent = agent->getPos() - m_vehicle->getPos();
    std::cout<<"agent position"<<agent->getPos()[0]<<"   "<<agent->getPos()[1]<<"   "<<agent->getPos()[2]<<std::endl;

//    if only want to conside pursuers within range
//    double threatRange = 100.0;
//    if(toAgent.lengthSquared() > threatRange * threatRange) return ngl::Vec3();

    double lookAheadTime = toAgent.length() / (m_vehicle->getMaxSpeed() + agent->getSpeed());
;
    ngl::Vec3 agentPos = agent->getPos() + (agent->getVelocity() * lookAheadTime);

    return Flee(agentPos);
}
