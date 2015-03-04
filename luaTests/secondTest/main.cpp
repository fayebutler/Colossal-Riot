#include <iostream>

extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

int main()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    //luaL_dofile(L, "/home/i7776126/compAnim02/colossal/luaTests/secondTest/main.lua");
    luaL_loadfile(L, "/home/i7776126/compAnim02/colossal/luaTests/secondTest/main.lua");
    lua_call(L, 0, 0);
    lua_close(L);
    std::cout << "Hello World! with C++" << std::endl;
    return 0;
    }

