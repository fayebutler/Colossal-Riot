#ifndef RIOT_GAME_H
#define RIOT_GAME_H

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

class Riot_Game
{
private:
    //lua_State* s_luaState;

    //Riot_Map* m_map;

    //std::list<Agent*> m_agents;

    //Squad* m_selectedSquad; //pointer to selected squad

    //std::list<Projectile*> m_Projectiles; //list of active projectiles

public:
    Riot_Game();
    ~Riot_Game();

    void render();
    void update();

    bool loadMap; // loads an environment from file

//    bool isPathObstructed(ngl::Vec2)

    std::vector<Agent*> getAllAgentsInFOV(const Agent* _agent)const;

    void loadBrain(std::string _brain);
    void addAgent(Agent * _agent);
    void addAgents(std::vector<Agent*> _agents);

    void clear();
    void restart();
};

#endif // RIOT_GAME_H
