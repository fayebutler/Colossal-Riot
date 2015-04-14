#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

class graphEdge //Encapsulates the basic information required to denote a connection between two graph nodes
{
protected:
    //GraphEdge connects 2 graphNodes. m_iFrom & m_iTo are graphNode indicies.
    int m_iFrom;
    int m_iTo;

    double m_dCost;
public:
    graphEdge(int from, int to, double cost):m_dCost(cost), m_iFrom(from), m_iTo(to)
    {}

    graphEdge():m_dCost(1.0),m_iFrom(-1),m_iTo(-1)
    {}

    graphEdge(int from, int to):m_dCost(1.0),m_iFrom(from),m_iTo(to)
    {}

    int From()const
    {
        return m_iFrom;
    }

    int To()const
    {
        return m_iTo;
    }

    virtual ~graphEdge()
    {}

};

#endif // GRAPHEDGE_H
