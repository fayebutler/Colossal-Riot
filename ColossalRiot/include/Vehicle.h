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

    ngl::Vec3 m_smoothHeading;
    std::vector <ngl::Vec3> m_headingHistory;
    int m_nextSlot;
    int m_sampleSize;
    bool m_smoothingOn;
    std::vector<ngl::Vec3> m_path;
    int m_pathIndex;





public:
    Vehicle(GameWorld* world, ngl::Vec3 position, ngl::Vec3 velocity, float rotation, float mass, float max_force, float max_speed, float max_turnrate, float scale);
    ~Vehicle();
    SteeringBehaviour* Steering(){return m_steering;}
    double TimeElapsed()const{return m_timeElapsed;}

    virtual bool handleMessage(const Message& _message);


    void update(double time_elapsed);
    void render();

    ngl::Vec3 getCrosshair()const{return m_crosshair;}
    void setCrosshair(ngl::Vec3 v){m_crosshair=v;}

    ngl::Vec3 smoothingUpdate(ngl::Vec3 m_recentHeading);

    ngl::Vec3 findNearestExit(std::vector<ngl::Vec3> _exits);
    void followPath();
    void findPath(ngl::Vec3 _target);

    int getPathIndex(){return m_pathIndex;}
    void setPathIndex(int _index){m_pathIndex = _index;}


    std::vector<ngl::Vec3> getPath(){return m_path;}
    void setPath(std::vector<ngl::Vec3> _path){m_path = _path;}


};

#endif // VEHICLE_H
