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
    std::vector <ngl::Vec3> m_exitPoints;
    std::vector<float> m_mapBounds;
    int m_numberOfCells;
    float m_cellSize;
    float m_maxDist; //Maximum distance from the centre of the cell within the cell.

public:
    CellGraph();
    CellGraph(const char *_fileName, int _borderSize);
    ~CellGraph();

    void printCellGraph();
    //call once at the start to put each agent into a cell
    bool entityIsInCell(int _cellID, BaseGameEntity *_entity);
    bool posIsInCell(ngl::Vec3 _pos);

    void initializeCells(BaseGameEntity *_entity);

    void updateCells(BaseGameEntity *_entity);
    void clearCells();
    void addEntities(BaseGameEntity *_entity);
    void generateWalls();

    std::vector<float> getMapBounds(){return m_mapBounds;}
    std::vector<ngl::Vec3> getExitPoints(){return m_exitPoints;}
    std::vector<ngl::Vec3> findPath(BaseGameEntity *_from, ngl::Vec3 _to); //Returns the centre of cells in path

};


#endif // CELL_H
