#ifndef BASEGAMEENTITY_H
#define BASEGAMEENTITY_H

#include <ngl/Transformation.h>
#include <vector>
#include <list>

#include "EntityManager.h"
#include "Message.h"
#include "MessageManager.h"
#include "Cell.h"
//#include "GameWorld.h"

class GameWorld;

enum entityType
{
  typeMovingEntity,
  typePolice,
  typeRioter,
  typeWall,
  typeSquad

};

class GameWorld;
class EntityManager;

class BaseGameEntity
{
protected:

  GameWorld* m_world;

  int m_ID;
  static int m_nextValidID;
  void setID(int _val) { m_ID = _val; }
  bool m_resetID;


  EntityManager* m_entityMgr;
  entityType m_entityType;
  ngl::Vec3 m_pos;
  float m_boundingRadius;

  Cell m_currentCell;
  int m_currentCellID;
  std::vector<int> m_detectedDynamicEntityIDs;
  float m_detectionRadius;


  std::vector<int> m_neighbourPoliceIDs;
  std::vector<int> m_neighbourRioterIDs;


public:
    BaseGameEntity();
    BaseGameEntity(GameWorld* world, entityType entity_type, ngl::Vec3 pos, float r);

    ~BaseGameEntity();

    int getID() const { return m_ID; }

    entityType getEntityType()const{return m_entityType;}
    void setEntityType(entityType newType){m_entityType = newType;}

    ngl::Vec3 getPos()const{return m_pos;}
    void setPos(ngl::Vec3 newPos) {m_pos = newPos;}

    float getBoundingRadius()const{return m_boundingRadius;}
    void setBoudingRadius(float r) {m_boundingRadius = r;}

    void addDetectedDynamicEntityID(int _ID);
    void clearDetectedDynamicEntityID();
    std::vector<int> getDetectedEntityIDs();

    void addPoliceID(int _ID);
    void addRioterID(int _ID);
    void clearAgentIDs();


    void setCurrentCellID(int _ID);
    int getCurrentCellID();
    void setCurrentCell(Cell _inCell);
    Cell getCurrentCell();

    float getDetectionRadius();
    void setDetectionRadius(float _r);

    std::vector<int> getNeighbourPoliceIDs() { return m_neighbourPoliceIDs; }
    std::vector<int> getNeighbourRioterIDs() { return m_neighbourRioterIDs; }

    virtual bool handleMessage(const Message& _message) = 0;

};

#endif // BASEGAMEENTITY_H
