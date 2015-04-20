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
    std::vector <int> m_neighbourIDs;
    ngl::Vec4 m_boundries;
    ngl::Vec3 m_centre;
    std::vector<int> m_agentsID;


public:

    Cell();
    Cell(int _ID, std::vector <ngl::Vec3> _fourCorners, std::vector<int> _neighbourIDs);
    void printCellInfo();
    std::vector<int> getNeighbourIDs();
    void addAgentID(int _ID);
    void clearAgentIDs();
    std::vector<int> getAgentIDs();


    ngl::Vec3 getCentre();
    float getSize();

};

#endif // CELL_H
