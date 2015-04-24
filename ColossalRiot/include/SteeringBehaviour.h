#ifndef STEERINGBEHAVIOUR_H
#define STEERINGBEHAVIOUR_H

#include <ngl/Transformation.h>
#include <vector>


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
    int m_activeFlags;
    ngl::Vec3 m_steeringForce;
    ngl::Vec3 m_target;
    Vehicle* m_targetAgent;


    float m_wanderDistance;
    float m_wanderRadius;
    float m_wanderJitter;

    int m_deceleration;

    ngl::Vec3 m_wanderTarget;

    float m_viewDistance;



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
    //need to add the rest
    ngl::Vec3 Seek(ngl::Vec3 TargetPos);
    ngl::Vec3 Flee(ngl::Vec3 TargetPos);
    ngl::Vec3 Arrive(ngl::Vec3 TargetPos, int deceleration);
    ngl::Vec3 Wander();
    ngl::Vec3 Pursuit(const Vehicle *agent);
    ngl::Vec3 Evade(const Vehicle* agent);

    ngl::Vec3 ObstacleAvoidance();

    ngl::Vec3 worldToLocalSpace(ngl::Vec3 pointWorldPos, ngl::Vec3 vehiclePos, ngl::Vec3 vehicleHeading, ngl::Vec3 vehicleSide);
    ngl::Vec3 localToWorldSpace(ngl::Vec3 pointLocalPos, ngl::Vec3 vehiclePos, ngl::Vec3 vehicleHeading);

// add in group steering behaviours
// add in collision avoidance


public:

    SteeringBehaviour(Vehicle* agent);

    ~SteeringBehaviour();

    void SeekOn(){m_activeFlags |= seek;} //bitwise OR
    void SeekOff(){if(on(seek)) m_activeFlags ^= seek;} //bitwise XOR
    bool isSeekOn(){return on(seek);}
    void setSeekWeight(double m_newWeight){m_weightSeek = m_newWeight;}
    double getSeekWeight(){return m_weightSeek;}

    void FleeOn(){m_activeFlags |= flee;}
    void FleeOff(){if(on(flee)) m_activeFlags ^= flee;}
    bool isFleeOn(){return on(flee);}
    void setFleeWeight(double m_newWeight){m_weightFlee = m_newWeight;}
    double getFleeWeight(){return m_weightFlee;}

    void ArriveOn(){m_activeFlags |= arrive;}
    void ArriveOff(){if(on(arrive)) m_activeFlags ^= arrive;}
    bool isArriveOn(){return on(arrive);}

    void WanderOn(){m_activeFlags |= wander;}
    void WanderOff(){if(on(wander)) m_activeFlags ^= wander;}
    bool isWanderOn(){return on(wander);}
    void setWanderWeight(double m_newWeight){m_weightWander = m_newWeight;}
    double getWanderWeight(){return m_weightWander;}

    void PursuitOn(){m_activeFlags |= pursuit;}
    void PursuitOff(){if(on(pursuit)) m_activeFlags ^= pursuit;}
    bool isPursuitOn(){return on(pursuit);}
    void setPursuitWeight(double m_newWeight){m_weightPursuit = m_newWeight;}
    double getPursuitWeight(){return m_weightPursuit;}

    void EvadeOn(){m_activeFlags |= evade;}
    void EvadeOff(){if(on(evade)) m_activeFlags ^= evade;}
    bool isEvadeOn(){return on(evade);}
    void setEvadeWeight(double m_newWeight){m_weightEvade = m_newWeight;}
    double getEvadeWeight(){return m_weightEvade;}

    void ObstacleAvoidOn() { m_activeFlags |= obstacle_avoidance; }
    void ObstacleAvoidOff() { if(on(obstacle_avoidance)) m_activeFlags ^= obstacle_avoidance; }
    bool isObstacleAvoidOn() { return on(obstacle_avoidance); }

    void setTarget(ngl::Vec3);
    void setTargetAgent(Vehicle* agent){m_targetAgent = agent;}


    ngl::Vec3 calculate();

    ngl::Vec3 calculateWeightedSum(); //simplest addition method, should update to prioritized


    double forwardComponent();
    double sideComponent();


};

#endif // STEERINGBEHAVIOUR_H
