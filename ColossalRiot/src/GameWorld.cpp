

#include "GameWorld.h"



GameWorld::GameWorld()
{
   ngl::Vec3 pos = ngl::Vec3(0,0,0);
   Vehicle* vehicle = new Vehicle(this,pos, ngl::Vec3(0,0,0), 0.0, 0.0, 10.0, 10.0,10.0,1.0);
   vehicle->Steering()->ArriveOn();
   vehicle->Steering()->FleeOn();
   vehicle->Steering()->SeekOn();

   m_vehicles.push_back(vehicle);

}
