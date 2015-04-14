#ifndef GRAPHNODE_H
#define GRAPHNODE_H

class graphNode //Graph node encapsulates the minimum information a node requires for an adjacency list graph representation.
{
protected:
    int m_iIndex;

public:
    graphNode():m_iIndex(-1){}
    graphNode(int idx):m_iIndex(idx){}

    virtual ~graphNode(){}

    int Index() const;
    void setIndex( int newIndex);

};

#endif // GRAPHNODE_H
