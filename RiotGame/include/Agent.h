#ifndef AGENT_H
#define AGENT_H

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

class Agent
{
private:
    lua_State* s_luaState;
    int s_numberOfAgents;
    float s_step;
    int m_agentID;
    float m_mass;
    float m_strength;

public:
    Agent();
    void setMass(float _mass);
    float getMass();
};
// poo
#endif // AGENT_H
