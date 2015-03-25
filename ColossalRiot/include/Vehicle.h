#ifndef VEHICLE_H
#define VEHICLE_H

#include <ngl/Transformation.h>
#include "MovingEntity.h"
#include "SteeringBehaviour.h"

class SteeringBehaviour;


class Vehicle: public MovingEntity
{
private:

    // gameWorld* m_gameWorld
    SteeringBehaviour* m_steering;
    double m_timeElapsed;

public:
    Vehicle(ngl::Vec3 position, ngl::Vec3 velocity, float rotation, float mass, float max_force, float max_speed, float max_turnrate, float scale);
    ~Vehicle();
    SteeringBehaviour* const Steering()const{return m_steering;}

    void update(double time_elapsed);
    void render();

};

#endif // VEHICLE_H
