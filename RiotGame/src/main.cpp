/// @file main.cpp
/// @brief construct the new MainWindow and pass control to it
#include "MainWindow.h"
#include <QApplication>

/* this code runs the basic main window and is created by the Qt Creator app */
#include "Agent.h"
#include <iostream>
#include "BaseGameEntity.h"
#include "MovingEntity.h"
#include "Vehicle.h"
#include "SteeringBehaviour.h"
#include <time.h>
#include "QTimer"
#include "Timer.h"
#include "SDL/SDL.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  // make an instance of the QApplication  
  QApplication a(argc, argv);
  // Create a new MainWindow
  MainWindow w;
  // show it


   w.show();



  // hand control over to Qt framework
//  lua_State *L = luaL_newstate();
//  luaL_openlibs(L);

//  luaL_loadfile(L, "lua/main.lua");

//  lua_call(L, 0, 0);
//  lua_close(L);


//  Agent newagent;
//  newagent.setMass(4.0);
//  std::cout<<newagent.getMass()<<std::endl;



//  MovingEntity move;
//  move.setMass(2.0);
//  move.setPos(ngl::Vec3(0.0,0.0,0.4));
//  std::cout<<"mass = "<<move.getMass()<<std::endl;
//  std::cout<<"pos = "<<move.getPos()[0]<<move.getPos()[1]<<move.getPos()[2]<<std::endl;



//  Vehicle* veh = new Vehicle(ngl::Vec3(5,5,5), ngl::Vec3(0,0,0), 0.0, 1.0, 10.0, 10.0, 10.0, 1.0);
//  veh->Steering()->SeekOn();
//  veh->Steering()->FleeOn();

//  std::cout<<"veh pos = "<<veh->getPos()[0]<<veh->getPos()[1]<<veh->getPos()[2]<<std::endl;








  return a.exec();
}
//----------------------------------------------------------------------------------------------------------------------
