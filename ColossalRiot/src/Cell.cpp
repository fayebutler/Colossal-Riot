//----------------------------------------------------------------------------------------------------------------------
///  @file Cell.cpp
///  @brief This the base cell class, used in CellGraph. It encapsulates all entities within it, neighbouring cells and walls as well.
//----------------------------------------------------------------------------------------------------------------------

#include "Cell.h"

//----------------------------------------------------------------------------------------------------------------------

Cell::Cell()
{

}
//----------------------------------------------------------------------------------------------------------------------

Cell::Cell(int _ID, std::vector<ngl::Vec3> _fourCorners, std::vector<int> _neighbourCellIDs,
           std::vector<int> _perpendicularCellIDs)
{
  // Checks there are 4 elements to the four corners parameter.
    if ( _fourCorners.size() == 4)
    {
    //Copy parameter into the member m_fourCorners:
    m_fourCorners = _fourCorners;
    //Set the bounds to the first element in _fourCorners,
    // this will be tested against later to find the correct bounds.
    float upper = _fourCorners[0].m_z,
          lower = _fourCorners[0].m_z,
          left = _fourCorners[0].m_x,
          right = _fourCorners[0].m_x;
    //For loop that compares each of the bounds to the elements of _fourCorners
    // and assigns the correct bounds accordingly:
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

    //m_boundaries are set accordingly:
    m_boundries.m_x = upper;
    m_boundries.m_y = lower;
    m_boundries.m_z = left;
    m_boundries.m_w = right;

    //Derrive m_centre from boundaries:
    m_centre.m_x = ((right+left)/2.0);
    m_centre.m_y = 0;
    m_centre.m_z = ((upper+lower)/2.0);

    //Set cell ID, neighbours and perpendicular neighbours:
    m_ID = _ID;
    m_neighbourCellIDs = _neighbourCellIDs;
    m_perpendicularNeighbourCellIDs = _perpendicularCellIDs;
    }

    else
    {
        std::cout<<" fourCorners does not have 4 corners!!"<<std::endl;
    }



}
//----------------------------------------------------------------------------------------------------------------------

Cell::~Cell()
{
  delete this;
}
