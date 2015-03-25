#include "SteeringBehaviour.h"
#include "Vehicle.h"
#include "GameWorld.h"
#include <cstdlib>
SteeringBehaviour::SteeringBehaviour(Vehicle* agent):
    m_vehicle(agent)
{

}



ngl::Vec3 SteeringBehaviour::calculate()
{
    //simple weight truncated sum
    ngl::Vec3 force;
    force += Seek(ngl::Vec3(0,0,0)) * m_weightSeek;
    force += Flee(ngl::Vec3(10,10,10)) *m_weightFlee;

    //use weight truncated running sum prioritisation
    //truncate force to maxforce
    return force;
}

ngl::Vec3 SteeringBehaviour::calculateWeightedSum()
{
    if(on(seek))
    {
        m_steeringForce += Seek(m_vehicle->World()->Crosshair()) * m_weightSeek;
    }
    if(on(flee))
    {
        m_steeringForce += Flee(m_vehicle->World()->Crosshair()) * m_weightFlee;
    }
    if(on(arrive))
    {
        m_steeringForce += Arrive(m_vehicle->World()->Crosshair(), m_deceleration) * m_weightArrive;
    }

   //truncate steering force to max force
    if(m_steeringForce.length() > m_vehicle->getMaxForce())
    {
        m_steeringForce.normalize();
        m_steeringForce = m_steeringForce * m_vehicle->getMaxForce();
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
    ngl::Vec3 desiredVelocity = ngl::Vec3(TargetPos - m_vehicle->getPos()) * m_vehicle->getMaxSpeed();
    desiredVelocity.normalize();
    return (desiredVelocity - m_vehicle->getVelocity());
}

ngl::Vec3 SteeringBehaviour::Flee(ngl::Vec3 TargetPos)
{
    ngl::Vec3 desiredVelocity = ngl::Vec3(m_vehicle->getPos() - TargetPos) * m_vehicle->getMaxSpeed();
    desiredVelocity.normalize();
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
//        speed = min(speed, m_vehicle->getMaxSpeed());

        ngl::Vec3 desiredVelocity = toTarget * speed/dist;
        return(desiredVelocity - m_vehicle->getVelocity());
    }

    return ngl::Vec3(0,0,0);
}

ngl::Vec3 SteeringBehaviour::Wander()
{
    double jitterTimeSlice = m_wanderJitter * m_vehicle->TimeElapsed() ;
    float randomClamped = -1+2*((float)rand())/RAND_MAX;
    m_wanderTarget += ngl::Vec3(randomClamped * jitterTimeSlice,0, randomClamped * jitterTimeSlice);

    m_wanderTarget.normalize();
    m_wanderTarget *= m_wanderRadius;
    ngl::Vec3 localTarget = m_wanderTarget + ngl::Vec3(m_wanderDistance,0,0);
  //  ngl::Vec3 worldTarget = pointtoworldspace ??

    //return worldTarget - m_vehicle->getPos();
}
