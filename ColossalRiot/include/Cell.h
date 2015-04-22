#ifndef CELL_H
#define CELL_H

#include <vector>
#include <glm/glm.hpp>
#include <ngl/Transformation.h>
#include <iostream>
#include <Agent.h>


class Cell
{
private:

    int m_ID;
    std::vector <ngl::Vec3> m_fourCorners;
    std::vector <int> m_neighbourCellIDs;
    ngl::Vec4 m_boundries;
    ngl::Vec3 m_centre;

    std::vector<int> m_dynamicEntityIDs;


public:

    Cell();
    Cell(int _ID, std::vector <ngl::Vec3> _fourCorners, std::vector<int> _neighbourCellIDs);
    void printCellInfo();
    std::vector<int> getNeighbourCellIDs();
    void addDynamicEntityID(int _ID);
    void clearDynamicEntityIDs();
    std::vector<int> getDynamicEntityIDs();


    ngl::Vec3 getCentre();
    float getSize();

};

#endif // CELL_H
