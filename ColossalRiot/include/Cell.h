#ifndef CELL_H
#define CELL_H

#include <vector>
#include <list>
#include <glm/glm.hpp>
#include <ngl/Transformation.h>
#include <iostream>

struct Wall
{
    ngl::Vec3 start;
    ngl::Vec3 end;
    ngl::Vec3 normal;
};

class Cell
{
private:

    int m_ID;
    std::vector <ngl::Vec3> m_fourCorners;
    std::vector <int> m_neighbourCellIDs;
    ngl::Vec4 m_boundries;
    ngl::Vec3 m_centre;

    std::vector<Wall> m_walls;

    std::vector<int> m_dynamicEntityIDs;


public:

    Cell();
    Cell(int _ID, std::vector <ngl::Vec3> _fourCorners, std::vector<int> _neighbourCellIDs);
    void printCellInfo();
    std::vector<int> getNeighbourCellIDs();
    void addDynamicEntityID(int _ID);
    void clearDynamicEntityIDs();
    std::vector<int> getDynamicEntityIDs();
    ngl::Vec4 getBoundaries(){ return m_boundries;}
    void addWall(Wall _wall);
    std::vector<Wall> getWalls(){return m_walls;}
    ngl::Vec3 getCentre();
    float getSize();

};

#endif // CELL_H
