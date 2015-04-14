
#include <iostream>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#include <LuaBridge.h>

using namespace luabridge;

int main()
{
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  luaL_loadfile(L, "main.lua");
  lua_call(L, 0, 0);

  LuaRef s = getGlobal(L, "testString");
  std::string luaString = s.cast<std::string>();
  std::cout<<luaString<<std::endl;

  lua_close(L);
  std::cout << "Hello World! with C++" << std::endl;
  return 0;
}

