#include "crowdengine.h"
#include <QApplication>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CrowdEngine w;
    w.show();

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    //  luaL_dofile(L, "lua/main.lua");
    luaL_loadfile(L, "/home/i7758474/Computing/colossal/CrowdEngine/CrowdEngine/lua/main.lua");
    //luaL_loadfile(L, "main.lua");
    lua_call(L, 0, 0);
    lua_close(L);

    return a.exec();
}
