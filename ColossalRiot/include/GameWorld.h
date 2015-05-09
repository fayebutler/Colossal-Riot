#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <ngl/Transformation.h>
#include <ngl/Camera.h>

#include "Rioter.h"
#include "Police.h"
#include "CellGraph.h"
#include "Squad.h"
#include "StaticEntity.h"

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

    //std::vector<BaseGameEntity*> m_obstacles;

    std::vector<Rioter*> m_rioters;
    std::vector<Squad*> m_squads;

    std::vector<StaticEntity*> m_obstacles;

    int m_initialNumberOfRioters;

    int m_numberOfRioters;
    int m_numberOfSquads;
    int m_numberOfObstacles;
    int m_numberOfTrees;
    int m_numberOfStreetLights;


    CellGraph* m_cellGraph;
    const char* m_cellGraphFile;

    ngl::Obj *m_streetMesh;
    std::string m_streetMeshFile;

    ngl::Obj *m_buildingMesh;
    std::string m_buildingMeshFile;

    ngl::Obj *m_policeMesh;
    ngl::Obj *m_rioterMesh;
    ngl::Obj *m_treeMesh;
    ngl::Obj *m_streetLightMesh;


    ngl::Vec3 m_policeStation;

    bool m_resetID;

    int m_availablePolice;
    int m_activePolice;

    bool m_win;
    bool m_lose;

    int m_numberOfRiotersDead;
    int m_numberOfRiotersHome;

    int m_numberOfRiotersDeadToLose;
    int m_numberOfRiotersHomeToWin;

    lua_State *L;




public:

    GameWorld(int _level);

    ~GameWorld();

    void loadMatricesToShader(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX);

    void Update(double timeElapsed, double currentTime);
    void draw(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX);

    std::vector<Rioter*> getRioters() { return m_rioters; }
    std::vector<Squad*> getSquads() {return m_squads;}

    void setPoliceStation(float _x, float _y, float _z);

    void createSquad(int size);

    int getAvailablePolice() const { return m_availablePolice; }
    void setAvailablePolice(int _availablePolice) { m_availablePolice = _availablePolice; }

    int getNumberOfRiotersDead() const { return m_numberOfRiotersDead; }
    void setNumberOfRiotersDead(int _number) { m_numberOfRiotersDead = _number; }
    int getNumberOfRiotersHome() const { return m_numberOfRiotersHome; }
    void setNumberOfRiotersHome(int _number) { m_numberOfRiotersHome = _number; }
    int getNumberOfRiotersDeadToLose() const { return m_numberOfRiotersDeadToLose; }
    void setNumberOfRiotersDeadToLose(int _number) { m_numberOfRiotersDeadToLose = _number; }
    int getNumberOfRiotersHomeToWin() const { return m_numberOfRiotersHomeToWin; }
    void setNumberOfRiotersHomeToWin(int _number) { m_numberOfRiotersHomeToWin = _number; }

    ngl::Vec3 getPoliceStation() const { return m_policeStation; }
   // void setPoliceStation(ngl::Vec3 _policeStation) { m_policeStation = _policeStation; }

    void setResetID(bool _resetID){m_resetID = _resetID;}
    bool getResetID()const{return m_resetID;}

    CellGraph* getCellGraph()const{return m_cellGraph;}

    void squadTarget(Squad *selectedSquad, ngl::Vec3 target);


    bool getHasWon()const{return m_win;}
    void setHasWon(bool _inBool) { m_win = _inBool; }
    bool getHasLost()const{return m_lose;}
    void setHasLost(bool _inBool) { m_lose = _inBool; }

    void registerLua(lua_State *_L);

    int getNumberOfRioters() {return m_numberOfRioters; }

    int getInitialNumberOfRioters() const { return m_initialNumberOfRioters; }
    void setInitialNumberOfRioters(int _number) {m_initialNumberOfRioters = _number; }

    int getNumberOfTrees() const { return m_numberOfTrees; }
    void setNumberOfTrees(int _number) {m_numberOfTrees = _number; }

    int getNumberOfStreetLights() const { return m_numberOfStreetLights; }
    void setNumberOfStreetLights(int _number) {m_numberOfStreetLights = _number; }

    const char* getCellGraphFile() const { return m_cellGraphFile; }
    void setCellGraphFile(const char* _file) { m_cellGraphFile = _file;}

    std::string getStreetMeshFile() const { return m_streetMeshFile; }
    std::string getBuildingMeshFile() const { return m_buildingMeshFile; }

    void setStreetMeshFile(std::string _file) { m_streetMeshFile = _file; }
    void setBuildingMeshFile(std::string file) { m_buildingMeshFile = file; }

};

#endif // GAMEWORLD_H
