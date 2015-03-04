#ifndef CROWDENGINE_H
#define CROWDENGINE_H

#include <cstdlib>
#include <string>
#include <vector>
#include <set>
#include "Agent.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

class CrowdEngine
{
private:
    lua_State* s_luaState;
    std::string s_brainsPath;
    std::vector<std::string> s_loadedBrains;
public:
    CrowdEngine();
    ~CrowdEngine();
    void loadBrain(std::string _brain);
    void addAgent(Agent * _agent);
    void addAgents(std::vector<Agent*> _agents);
    void update();
    void clear();
    void restart();
};

#endif // CROWDENGINE_H
