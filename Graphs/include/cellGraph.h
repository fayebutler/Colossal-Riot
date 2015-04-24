#ifndef CELLGRAPH_H
#define CELLGRAPH_H

#include <fstream>
#include <sstream>
#include <string>
#include <ngl/Transformation.h>
#include "cell.h"

class cellGraph
{
private:
    std::vector <cell> m_cells;


public:
    cellGraph();
    cellGraph(const char *_fileName);
    void printCellGraph();

};


#endif // CELL_H
