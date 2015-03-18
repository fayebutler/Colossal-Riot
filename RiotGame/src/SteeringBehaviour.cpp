#include "SteeringBehaviour.h"
#include "Vehicle.h"

SteeringBehaviour::SteeringBehaviour()
{

}


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

double SteeringBehaviour::forwardComponent()
{
    return m_vehicle->getHeading().dot(m_steeringForce);
}

double SteeringBehaviour::sideComponent()
{
    return m_vehicle->getSide().dot(m_steeringForce);
}
