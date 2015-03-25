#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include
#include "BaseGameEntity.h"
//#include ""
//#include "cellSpacePartition"
#include <ngl/Transformation.h>

//class
//
//


class GameWorld
{
private:
    //std::vector of vehicles

    //any obstacles
    std::vector<BaseGameEntity*> m_obstacles;

    //std::vector<Wall2D> m_Walls;
};

#endif // GAMEWORLD_H
