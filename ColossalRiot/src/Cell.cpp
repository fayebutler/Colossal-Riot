#include "Cell.h"

Cell::Cell()
{

}

Cell::Cell(int _ID, std::vector<ngl::Vec3> _fourCorners, std::vector<int> _neighbourCellIDs,
           std::vector<int> _perpendicularCellIDs)
{
    if ( _fourCorners.size() == 4)
    {
    m_fourCorners = _fourCorners;
    //creating the bounds
    float upper = _fourCorners[0].m_z,
          lower = _fourCorners[0].m_z,
          left = _fourCorners[0].m_x,
          right = _fourCorners[0].m_x;
      for (unsigned int i=0; i<4 ;i++)
      {
          if (upper> _fourCorners[i].m_z)
          {
              upper= _fourCorners[i].m_z;
          }
          if (lower< _fourCorners[i].m_z)
          {
              lower= _fourCorners[i].m_z;
          }
          if (left> _fourCorners[i].m_x)
          {
              left= _fourCorners[i].m_x;
          }
          if (right< _fourCorners[i].m_x)
          {
              right= _fourCorners[i].m_x;
          }
    }

    m_boundries.m_x = upper;
    m_boundries.m_y = lower;
    m_boundries.m_z = left;
    m_boundries.m_w = right;

    m_centre.m_x = ((right+left)/2.0);
    m_centre.m_y = 0;
    m_centre.m_z = ((upper+lower)/2.0);

    m_ID = _ID;
    m_neighbourCellIDs = _neighbourCellIDs;
    m_perpendicularNeighbourCellIDs = _perpendicularCellIDs;
    }
    else
    {
        std::cout<<" fourCorners does not have 4 corners!!"<<std::endl;
    }



}

std::vector <int> Cell::getNeighbourCellIDs()
{
    return m_neighbourCellIDs;
}


ngl::Vec3 Cell::getCentre()
{
    return m_centre;
}

float Cell::getSize()
{

    return fabs(m_boundries[0] - m_boundries[1]);
}

void Cell::addDynamicEntityID(int _ID)
{
    m_dynamicEntityIDs.push_back(_ID);
}

void Cell::clearDynamicEntityIDs()
{
    m_dynamicEntityIDs.clear();
}

std::vector<int> Cell::getDynamicEntityIDs()
{
    return m_dynamicEntityIDs;

}

void Cell::addWall(Wall _wall)
{
    m_walls.push_back(_wall);
}
void Cell::addWallInCell(Wall _wall)
{
    m_wallsInCell.push_back(_wall);
}

void Cell::addPerpendicularNeighbourID(int _ID)
{
    m_perpendicularNeighbourCellIDs.push_back(_ID);
}

std::vector<int> Cell::getPerpendicularNeighbourCellIDs()
{
    return m_perpendicularNeighbourCellIDs;
}
