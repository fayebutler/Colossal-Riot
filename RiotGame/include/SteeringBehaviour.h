#ifndef STEERINGBEHAVIOUR_H
#define STEERINGBEHAVIOUR_H

#include <ngl/Transformation.h>

class Vehicle;
class BaseGameEntity;

class SteeringBehaviour
{

private:

    //enumerate list of behaviours
    enum behaviour_type
    {
        seek    = 0x00002,
        flee    = 0x00004
    };
private:

    Vehicle* m_vehicle;
    ngl::Vec3 m_steeringForce;
    ngl::Vec3 m_target;

    float m_wanderJitter;
    float m_wanderRadius;
    float m_wanderDistance;
    ngl::Vec3 m_wanderTarget;

    float m_viewDistance;

    int m_activeFlags;

    bool on(behaviour_type bt){return (m_activeFlags & bt) == bt;}

    //multipliers to adjust strength of behaviour
    double m_weightSeek;
    double m_weightFlee;

//  behaviour type functions
    ngl::Vec3 Seek(ngl::Vec3 TargetPos);
    ngl::Vec3 Flee(ngl::Vec3 TargetPos);

public:
    SteeringBehaviour();
    ~SteeringBehaviour();

    void SeekOn(){m_activeFlags |= seek;} //bitwise OR
    void SeekOff(){if(on(seek)) m_activeFlags ^= seek;} //bitwise XOR
    bool isSeekOn(){return on(seek);}

    void FleeOn(){m_activeFlags |= seek;}
    void FleeOff(){if(on(flee)) m_activeFlags ^= flee;}
    bool isFleeOn(){return on(flee);}

    void setTarget(ngl::Vec3);
//    void setTargetAgent(Vehicle* agent);


    ngl::Vec3 calculate();

    double forwardComponent();
    double sideComponent();




};

#endif // STEERINGBEHAVIOUR_H
