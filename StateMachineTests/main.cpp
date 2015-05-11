#include <iostream>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

int main()
{
    std::cout << "Starting C++" << std::endl;

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    //  luaL_dofile(L, "main.lua");
    luaL_loadfile(L, "main.lua");
    lua_call(L, 0, 0);
    lua_close(L);


    std::cout << "Back to C++" << std::endl;
    return 0;
}

