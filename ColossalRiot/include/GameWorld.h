#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <ngl/Transformation.h>
#include <ngl/Camera.h>

#include "Rioter.h"
#include "Police.h"
#include "CellGraph.h"
#include "Squad.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "LuaBridge.h"

typedef std::vector<BaseGameEntity*>::iterator ObIt;

class GameWorld
{
private:

    //entities that update each frame (not walls n shit)

    EntityManager* m_entityMgr;

    std::vector<int> m_dynamicEntities;

    std::vector<BaseGameEntity*> m_obstacles;

    std::vector<Rioter*> m_rioters;
    std::vector<Squad*> m_squads;

    int m_initialNumberOfRioters;

    int m_numberOfRioters;
    int m_numberOfSquads;


    CellGraph* m_cellGraph;
    const char* m_cellGraphFile;
    ngl::Obj *m_worldMesh;
    std::string m_worldMeshFile;
    ngl::Obj *m_policeMesh;
    ngl::Obj *m_rioterMesh;

    ngl::Vec3 m_policeStation;

    bool m_resetID;

    int m_availablePolice;
    int m_activePolice;

    bool m_win;
    bool m_lose;

    int m_numberOfRiotersDead;
    int m_numberOfRiotersHome;



    lua_State *L;




public:

    GameWorld(int _level);

    ~GameWorld();

    void loadMatricesToShader(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX);

    void Update(double timeElapsed, double currentTime);
    void draw(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX);

    std::vector<Rioter*> getRioters() { return m_rioters; }
    //std::vector<Police*> getPolice() { return m_police; }

    std::vector<Squad*> getSquads() {return m_squads;}

    void setPoliceStation(float _x, float _y, float _z);

    void createSquad(int size);

    int getAvailablePolice() const { return m_availablePolice; }
    void setAvailablePolice(int _availablePolice) { m_availablePolice = _availablePolice; }

    void setResetID(bool _resetID){m_resetID = _resetID;}
    bool getResetID()const{return m_resetID;}

    CellGraph* getCellGraph()const{return m_cellGraph;}

    void squadTarget(Squad *selectedSquad, ngl::Vec3 target);


    bool hasWon()const{return m_win;}
    bool hasLost()const{return m_lose;}


    void registerLua(lua_State *_L);

    int getNumberOfRioters() {return m_numberOfRioters; }

    int getInitialNumberOfRioters() const { return m_initialNumberOfRioters; }
    void setInitialNumberOfRioters(int _number) {m_initialNumberOfRioters = _number; }

    const char* getCellGraphFile() const { return m_cellGraphFile; }
    void setCellGraphFile(const char* _file) { m_cellGraphFile = _file; }
    std::string getWorldMeshFile() const { return m_worldMeshFile; }
    void setWorldMeshFile(std::string _file) { m_worldMeshFile = _file; }

};

#endif // GAMEWORLD_H
