#include "sparsegraph.h"

sparseGraph::sparseGraph()
{
    m_iNextNodeIndex = 0;
}

const graphNode& sparseGraph::getNode(int idx)const
{
    return m_Nodes[idx];
}

graphNode& sparseGraph::getNode(int idx)
{
    return m_Nodes[idx];
}

const graphEdge& sparseGraph::getEdge(int from, int to) const
{
    edgeList From = m_EdgeListVector[from];
    edgeList To = m_EdgeListVector[to];

    for (int i =0;i<From.size();i++)
    {
        graphEdge e = From[i];

        if (e.From() == from && e.To() == to)
        {
            return e;
        }

    }
}

graphEdge& sparseGraph::getEdge(int from, int to)
{
    edgeList From = m_EdgeListVector[from];
    edgeList To = m_EdgeListVector[to];

    for (int i =0;i<From.size();i++)
    {
        graphEdge e = From[i];

        if (e.From() == from && e.To() == to)
        {
            return e;
        }
    }
}

int sparseGraph::getNextFreeNodeIndex() const
{
    return m_iNextNodeIndex;
}

int sparseGraph::addNode(graphNode node)
{
   m_Nodes.push_back(node);
   m_iNextNodeIndex = m_Nodes.size();
   return m_iNextNodeIndex-1;

}

void sparseGraph::removeNode(int node)
{
    std::vector<graphNode>::iterator pos = m_Nodes.begin() + node;
    m_Nodes.erase(pos);
}

void sparseGraph::addEdge(graphEdge edge)
{
    int From = edge.From();
    int To = edge.To();

    m_EdgeListVector[From].push_back(edge);
    m_EdgeListVector[To].push_back(edge);
}

void sparseGraph::removeEdge(int from, int to)
{

        for (int i=0; i<m_EdgeListVector[from].size();i++)
        {
            if (m_EdgeListVector[from][i].To() == to )
            {
                std::vector<graphEdge>::iterator curEdge =m_EdgeListVector[from].begin() + i ;
                m_EdgeListVector[from].erase(curEdge);
                break;
            }
        }
}

int sparseGraph::numActiveNodes()const
{
    return m_Nodes.size();
}

int sparseGraph::numEdges()const
{
    int edges;

    for (int i=0;i<m_EdgeListVector.size();i++)
    {
      edges += m_EdgeListVector[i].size();
    }

    return edges;
}




