/// @file main.cpp
/// @brief construct the new MainWindow and pass control to it

#include <QApplication>
#include "MainWindow.h"
/* this code runs the basic main window and is created by the Qt Creator app */
#include "Agent.h"
#include <iostream>

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
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  luaL_loadfile(L, "lua/main.lua");

  lua_call(L, 0, 0);
  lua_close(L);

  Agent newagent;
  newagent.setMass(4.0);
  std::cout<<newagent.getMass()<<std::endl;



  return a.exec();
}
//----------------------------------------------------------------------------------------------------------------------
