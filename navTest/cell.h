#ifndef CELL_H
#define CELL_H

#include <vector>
#include <glm/glm.hpp>
#include <ngl/Transformation.h>

class cell
{
public:

    cell(int ID);

    int m_ID;
    std::vector <ngl::Vec4> m_fourCorners;
    std::vector <int> m_neighbours;
    float m_centre;


private:



};

#endif // CELL_H
