#include "Doom.h"

Doom::Doom()
{
    m_age = 23;
    m_name = "DOOOOM";
    m_health = 100;

    doom_L = luaL_newstate();
    //luaL_dofile(doom_L, "main.lua");
    luaL_openlibs(doom_L);

    luaRegister(doom_L);
    luabridge::push(doom_L, this);
    lua_setglobal(doom_L, "owner");

    //lua_pcall(doom_L, 0, 0, 0);


}

void Doom::printAge()
{
    std::cout<<"age is "<<m_age<<std::endl;
}

void Doom::luaRegister(lua_State* inL)
{
    luabridge::getGlobalNamespace(inL)
            .beginClass<Doom>("Doom")
                .addConstructor <void (*) (void)> ()
                    .addData("m_age", &Doom::m_age)
                    .addFunction("printAge", &Doom::printAge)
                    .addFunction("setAge", &Doom::setAge)
            .endClass();
}

void Doom::doLua()
{
//    push(doom_L, this);
//    lua_setglobal(doom_L, "owner");

        luaL_dofile(doom_L, "main.lua");
//        luaL_openlibs(doom_L);
        lua_pcall(doom_L, 0, 0, 0);
//        lb::LuaRef s = getGlobal(doom_L, "testString");
//        lb::LuaRef n = getGlobal(doom_L, "number");
//        std::string luaString = s.cast<std::string>();
//        int answer = n.cast<int>();
//        std::cout << luaString << std::endl;
//        std::cout << "And here's our number:" << answer << std::endl;

//        lb::LuaRef age = getGlobal(doom_L, "t");
//        m_age = age;
}
