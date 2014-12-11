#include <iostream>

//extern "C" {
//#include "lua/src/lua.h"
//#include "lua/src/lualib.h"
//#include "lua/src/lauxlib.h"
//}

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <luaconf.h>
}


int main()
{
    std::cout << "Hello World! with C++" << std::endl;

    lua_State *L =  lua_open();//q  luaL_newstate();
//    luaL_openlibs(L);
    luaL_loadfile(L, "lua/main.lua");
    //luaL_dofile(L, "lua/main.lua");
    lua_call(L, 0, 0);
    //lua_pcall(L, 0, 0, 0);
    lua_close(L);
    //std::cin.get();
    return 0;
}

