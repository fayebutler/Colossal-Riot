#ifndef VEHICLE_H
#define VEHICLE_H

#include <ngl/Transformation.h>
#include "MovingEntity.h"
#include "SteeringBehaviour.h"

class SteeringBehaviour;

class GameWorld;


class Vehicle: public MovingEntity
{
private:

    GameWorld* m_world;

    SteeringBehaviour* m_steering;

    double m_timeElapsed;

public:
    Vehicle(GameWorld* world, ngl::Vec3 position, ngl::Vec3 velocity, float rotation, float mass, float max_force, float max_speed, float max_turnrate, float scale);
    ~Vehicle();
    SteeringBehaviour* const Steering()const{return m_steering;}
    GameWorld* const World()const{return m_world;}
    double TimeElapsed()const{return m_timeElapsed;}

    virtual bool handleMessage(const Message& _message);


    void update(double time_elapsed);
    void render();

};

#endif // VEHICLE_H
