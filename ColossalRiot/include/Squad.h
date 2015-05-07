#ifndef SQUAD_H
#define SQUAD_H

#include "Police.h"

enum eSquadState
{
  squadPatrol,
  squadAggressive,
  squadDefensive,
  squadWall
};

class Squad : public Vehicle
{
public:
    Squad(GameWorld *world, int squadSize, ngl::Vec3 pos, float r);
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

    void setPath(std::vector<ngl::Vec3> _path);
    std::vector<ngl::Vec3> getPath() const {return m_path;}


    void findClosestWalls(Squad* squad);
    void formWall();

    int checkDeaths();

    void setSquadState(const char* _luaState, eSquadState _enumState);
    eSquadState getSquadState() { return m_squadState; }



private:

    int m_squadSize;
    float m_squadRadius;
//    ngl::Vec3 m_squadPos;

    ngl::Vec3 m_selectionColour;

    static ngl::Vec3 s_nextSelectionColour;

    std::vector <Police*> m_squadPolice;

    ngl::Colour m_squadColour;

    std::vector<ngl::Vec3> m_path;
    ngl::Vec3 averagePolicePos();

    int m_pathIndex;
    //int m_policeArrived;

    std::vector <bool> m_policeArrived;
    bool m_allArrived;
    bool m_foundWall;

    std::vector<Wall> m_closestWalls;

    eSquadState m_squadState;
};

#endif // SQUAD_H
