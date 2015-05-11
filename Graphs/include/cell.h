#ifndef CELL_H
#define CELL_H

#include <vector>
#include <glm/glm.hpp>
#include <ngl/Transformation.h>
#include <iostream>

class cell
{
private:

    int m_ID;
    std::vector <ngl::Vec3> m_fourCorners;
    std::vector <int> m_neighbourIDs;
    ngl::Vec4 m_boundries;
    ngl::Vec3 m_centre;


public:

    cell();
    cell(int _ID, std::vector <ngl::Vec3> _fourCorners, std::vector<int> _neighbourIDs);
    void printCellInfo();
    std::vector<int> getNeighbourIDs();

};

#endif // CELL_H
