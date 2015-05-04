#ifndef VEHICLE_H
#define VEHICLE_H

#include <ngl/Transformation.h>
#include "MovingEntity.h"
#include "SteeringBehaviour.h"

class SteeringBehaviour;


class Vehicle: public MovingEntity
{
private:


    SteeringBehaviour* m_steering;


    double m_timeElapsed;

      ngl::Vec3 m_crosshair;



public:
    Vehicle(GameWorld* world, ngl::Vec3 position, ngl::Vec3 velocity, float rotation, float mass, float max_force, float max_speed, float max_turnrate, float scale);
    ~Vehicle(){;}
    SteeringBehaviour* Steering(){return m_steering;}
//    GameWorld* World(){return m_world;}
    double TimeElapsed()const{return m_timeElapsed;}

    virtual bool handleMessage(const Message& _message);


    void update(double time_elapsed);
    void render();

    ngl::Vec3 getCrosshair()const{return m_crosshair;}
    void setCrosshair(ngl::Vec3 v){m_crosshair=v;}

};

#endif // VEHICLE_H
