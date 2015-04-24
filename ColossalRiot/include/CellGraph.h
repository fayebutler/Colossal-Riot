#ifndef CELLGRAPH_H
#define CELLGRAPH_H

#include <fstream>
#include <sstream>
#include <string>
#include <ngl/Transformation.h>
#include "Cell.h"



class CellGraph
{
private:
    std::vector <Cell> m_cells;
    float m_cellSize;
    float m_maxDist; //Maximum distance from the centre of the cell within the cell.


public:
    CellGraph();
    CellGraph(const char *_fileName);
    void printCellGraph();
    void updateCells(BaseGameEntity *_entity);
    void clearCells();
    void addEntities(BaseGameEntity *_entity);

};


#endif // CELL_H