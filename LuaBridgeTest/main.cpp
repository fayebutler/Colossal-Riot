#include <iostream>

#include "Doom.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <LuaBridge.h>

namespace lb = luabridge;
using namespace lb;

int main() {
//    lua_State* L = luaL_newstate();
//    luaL_dofile(L, "main.lua");
//    luaL_openlibs(L);
//    lua_pcall(L, 0, 0, 0);

//    lb::LuaRef s = getGlobal(L, "testString");
//    lb::LuaRef n = getGlobal(L, "number");
//    std::string luaString = s.cast<std::string>();
//    int answer = n.cast<int>();
//    std::cout << luaString << std::endl;
//    std::cout << "And here's our number:" << answer << std::endl;


//    Doom doomA;
//    doomA.luaRegister(doomA.getDoomL());
//    Doom doomB;
//    doomB.luaRegister(doomB.getDoomL());

//    std::cout<<"doomA Age = "<<doomA.getAge()<<std::endl;
//    std::cout<<"doomB Age = "<<doomB.getAge()<<std::endl;
//    doomA.doLua();
//    std::cout<<"doomA Age = "<<doomA.getAge()<<std::endl;
//    std::cout<<"doomB Age = "<<doomB.getAge()<<std::endl;


    Doom doomA;
   // doomA.luaRegister(doomA.getDoomL());

    std::cout<<"doomA age: "<<doomA.getAge()<<std::endl;

    doomA.doLua();

    std::cout<<"doomA age: "<<doomA.getAge()<<std::endl;

    Doom doomB;

    std::cout<<"doomB age: "<<doomB.getAge()<<std::endl;
    doomB.doLua();
    std::cout<<"doomB age: "<<doomB.getAge()<<std::endl;

    return 0;
}
