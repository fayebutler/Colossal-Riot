#ifndef GAMEWORLD_H
#define GAMEWORLD_H


#include "BaseGameEntity.h"

#include <ngl/Transformation.h>
#include <ngl/Camera.h>

#include "Rioter.h"
#include "Police.h"
#include "CellGraph.h"

typedef std::vector<BaseGameEntity*>::iterator ObIt;

class GameWorld
{
private:

    //entities that update each frame (not walls n shit)

    std::vector<int> m_dynamicEntities;

    std::vector<BaseGameEntity*> m_obstacles;

    std::vector<Rioter*> m_rioters;
    std::vector<Police*> m_police;



    CellGraph m_cellGraph;


public:
    GameWorld();
    ~GameWorld();



    void Update(double timeElapsed, double currentTime);
    void draw(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX);

    std::vector<Rioter*> getRioters() { return m_rioters; }
    std::vector<Police*> getPolice() { return m_police; }

    //const std::vector<Vehicle*>& Agents(){return m_vehicles;}

    //std::vector<Wall2D> m_Walls;
};

#endif // GAMEWORLD_H
