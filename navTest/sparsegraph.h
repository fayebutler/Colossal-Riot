#ifndef SPARSEGRAPH_H
#define SPARSEGRAPH_H
#include <vector>
#include <list>
#include <algorithm>
#include "graphedge.h"
#include "graphnode.h"


class sparseGraph // Groups graphEdge and graphNode classes. The class is designed so that each node's index number
                  //keys directly to a vector of graph nodes and a vector of edge lists.
{

    typedef std::vector <graphEdge> edgeList;

private:
    std::vector <graphNode> m_Nodes;
    std::vector <edgeList> m_EdgeListVector; //adjacency edge lists, each node index keys into the list of edges associated with the node.

    int m_iNextNodeIndex; //Index of the next node to be added.
public:

    sparseGraph();
    const graphNode& getNode( int idx)const;
    graphNode& getNode(int idx);
    const graphEdge& getEdge(int from, int to)const;
    graphEdge& getEdge(int from, int to);


    int getNextFreeNodeIndex()const;
    int addNode(graphNode node);
    void removeNode( int node);
    void addEdge(graphEdge edge);
    void removeEdge(int from, int to);

    int numActiveNodes() const;
    int numEdges()const;

};

#endif // SPARSEGRAPH_H
