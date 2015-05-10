#ifndef SQUAD_H
#define SQUAD_H

#include "Police.h"

enum eSquadState
{
  squadPatrol,
  squadAggressive,
  squadDefensive,
  squadWall,
  squadMove
};

class Squad : public Vehicle
{
public:
    Squad(GameWorld *world, int squadSize, ngl::Vec3 pos, float r, ngl::Obj* _mesh);
    ~Squad();

//    lua_State* getLuaState() const { return L ; }

    void update(double timeElapsed, double currentTime);
    void draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX);
    void loadMatricesToShader(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX);
    bool handleMessage(const Message& _message);

    int getSquadSize() const { return m_squadSize; }
    void setSquadSize(int _val) { m_squadSize = _val; }

    float getSquadRadius() const { return m_squadRadius; }
    void setSquadRadius(float _val) { m_squadRadius = _val; }

//    ngl::Vec3 getSquadPos() const { return m_squadPos; }
//    void setSquadPos(ngl::Vec3 _val) { m_squadPos = _val; }
    bool checkSelectionColour(const ngl::Vec3 colour);
    void selectionDraw(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX);

    ngl::Colour getSquadColour() const { return m_squadColour; }
    void setSquadColour(ngl::Colour _val) { m_squadColour = _val; }

    std::vector<Police*> getSquadPolice() const {return m_squadPolice;}

    void setTarget(ngl::Vec3 _target);
    ngl::Vec3 getTarget() const {return m_target;}


    void findClosestWalls(Squad* squad);
    void formWall();

    int checkDeaths();

    void setSquadState(const char* _luaState, eSquadState _enumState);
    eSquadState getSquadState() { return m_squadState; }



private:

    int m_squadSize;
    float m_squadRadius;
//    ngl::Vec3 m_squadPos;
    ngl::Obj* m_mesh;
    ngl::Vec3 m_selectionColour;

    static ngl::Vec3 s_nextSelectionColour;

    std::vector <Police*> m_squadPolice;
    std::vector <Police*> m_deadSquadPolice;

    ngl::Colour m_squadColour;

    ngl::Vec3 m_target;

    bool m_foundTarget;
    ngl::Vec3 averagePolicePos();

//    int m_pathIndex;
    //int m_policeArrived;

    std::vector <bool> m_policeArrived;
    bool m_allArrived;
    bool m_generatedBlockade;
    bool m_inBlockade;


    std::vector<Wall> m_closestWalls;
    std::vector<ngl::Vec3> m_blockadePositions;

    eSquadState m_squadState;
    eSquadState m_previousState;
};

#endif // SQUAD_H
