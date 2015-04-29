#ifndef CELLGRAPH_H
#define CELLGRAPH_H

#include <fstream>
#include <sstream>
#include <string>
#include <ngl/Transformation.h>
#include "Cell.h"
#include "EntityManager.h"
#include <ngl/Obj.h>

class CellGraph
{
private:

    EntityManager* m_entityMgr;

    std::vector <Cell> m_cells;
    int m_numberOfCells;
    float m_cellSize;
    float m_maxDist; //Maximum distance from the centre of the cell within the cell.


public:
    CellGraph();
    CellGraph(const char *_fileName);
    void printCellGraph();
    //call once at the start to put each agent into a cell
    bool entityIsInCell(int _cellID, BaseGameEntity *_entity);

    void initializeCells(BaseGameEntity *_entity);

    void updateCells(BaseGameEntity *_entity);
    void clearCells();
    void addEntities(BaseGameEntity *_entity);
    void generateWalls();

};


#endif // CELL_H
