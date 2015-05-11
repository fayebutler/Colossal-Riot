#ifndef GAMEWORLD_H
#define GAMEWORLD_H


#include "BaseGameEntity.h"
<<<<<<< HEAD
//#include ""
=======
>>>>>>> a21cc536f7ad22abe749ab110b5ca0c4b39210f2
//#include "cellSpacePartition"
#include <ngl/Transformation.h>

#include "Vehicle.h"
#include <vector>
//class
//
//

//typedef std::vector<BaseGameEntity*>::iterator ObIt;

class GameWorld
{
private:
    std::vector<BaseGameEntity*> m_obstacles;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
    std::vector<Vehicle*> m_vehicles;

    ngl::Vec3 m_crosshair;

public:
    GameWorld();
    ~GameWorld();

    ngl::Vec3 Crosshair()const{return m_crosshair;}
    void setCrosshair(ngl::Vec3 v){m_crosshair=v;}

    void Update(double timeElapsed);
    void Render();

    const std::vector<Vehicle*>& Agents(){return m_vehicles;}
>>>>>>> faye

>>>>>>> a21cc536f7ad22abe749ab110b5ca0c4b39210f2
    //std::vector<Wall2D> m_Walls;
};

#endif // GAMEWORLD_H
