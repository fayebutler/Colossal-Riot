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
        seek               = 0x00002,
        flee               = 0x00004,
        arrive             = 0x00008,
        wander             = 0x00010,
        cohesion           = 0x00020,
        separation         = 0x00040,
        allignment         = 0x00080,
        obstacle_avoidance = 0x00100,
        wall_avoidance     = 0x00200,
        follow_path        = 0x00400,
        pursuit            = 0x00800,
        evade              = 0x01000,
        interpose          = 0x02000,
        hide               = 0x04000,
        flock              = 0x08000,
        offset_pursuit     = 0x10000,

    };
private:

    Vehicle* m_vehicle;
    ngl::Vec3 m_steeringForce;
    ngl::Vec3 m_target;

    float m_wanderJitter;
    float m_wanderRadius;
    float m_wanderDistance;
    int m_deceleration;

    ngl::Vec3 m_wanderTarget;

    float m_viewDistance;

    int m_activeFlags;

    bool on(behaviour_type bt){return (m_activeFlags & bt) == bt;}

    //multipliers to adjust strength of behaviour
    double m_weightSeek;
    double m_weightFlee;

    double m_weightArrive;
    double m_weightWander;
    double m_weightCohesion;
    double m_weightSeparation;
    double m_weightAlignment;
    double m_weightObstacleAvoidance;
    double m_weightWallAvoidance;
    double m_weightFollowPath;
    double m_weightPursuit;
    double m_weightEvade;
    double m_weightInterpose;
    double m_weightHide;
    double m_weightFlock;
    double m_weightOffsetPursuit;


//  behaviour type functions
    ngl::Vec3 Seek(ngl::Vec3 TargetPos);
    ngl::Vec3 Flee(ngl::Vec3 TargetPos);
    ngl::Vec3 Arrive(ngl::Vec3 TargetPos, int deceleration);


public:
    SteeringBehaviour(Vehicle* agent);

    ~SteeringBehaviour();

    void SeekOn(){m_activeFlags |= seek;} //bitwise OR
    void SeekOff(){if(on(seek)) m_activeFlags ^= seek;} //bitwise XOR
    bool isSeekOn(){return on(seek);}

    void FleeOn(){m_activeFlags |= seek;}
    void FleeOff(){if(on(flee)) m_activeFlags ^= flee;}
    bool isFleeOn(){return on(flee);}

    void ArriveOn(){m_activeFlags |= arrive;}
    void ArriveOff(){if(on(arrive)) m_activeFlags ^= arrive;}
    bool isArriveOn(){return on(arrive);}

    void WanderOn(){m_activeFlags |= wander;}
    void WanderOff(){if(on(wander)) m_activeFlags ^= wander;}
    bool isWanderOn(){return on(wander);}

    void setTarget(ngl::Vec3);
//    void setTargetAgent(Vehicle* agent);


    ngl::Vec3 calculate();

    ngl::Vec3 calculateWeightedSum(); //simplest addition method, should update to prioritized


    double forwardComponent();
    double sideComponent();




};

#endif // STEERINGBEHAVIOUR_H