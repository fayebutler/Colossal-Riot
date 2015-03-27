#ifndef DOOM_H
#define DOOM_H

#include <iostream>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <LuaBridge.h>


class Doom
{
public:
    Doom();

    std::string getName(){return m_name;}
    void setName(std::string _name){ m_name = _name ;}

    int getAge(){return m_age;}
    void setAge(int _age){ m_age = _age ;}

    float getHealth(){return m_health;}
    void setHealth(float _health){ m_health = _health;}

    lua_State* getDoomL(){return doom_L;}
    void setDoomL(lua_State* _doom_L){ doom_L = _doom_L; }

    void printAge();

    static void luaRegister(lua_State* inL);

    void doLua();

private:
    std::string m_name;
    int m_age;
    float m_health;
    lua_State* doom_L;
};



#endif // DOOM_H
