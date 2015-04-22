#ifndef BASEGAMEENTITY_H
#define BASEGAMEENTITY_H

#include <ngl/Transformation.h>
#include "EntityManager.h"
#include "Message.h"
#include "MessageManager.h"

enum entityType
{
  typeMovingEntity,
  typePolice,
  typeRioter,
  typeWall

};

class BaseGameEntity
{
protected:
  int m_ID;
  static int m_nextValidID;
  void setID(int _val) { m_ID = _val; }
  entityType m_entityType;
  ngl::Vec3 m_pos;
  float m_boundingRadius;

public:
    BaseGameEntity();
    BaseGameEntity(entityType entity_type, ngl::Vec3 pos, float r);

 //   ~BaseGameEntity();

    int getID() const { return m_ID; }

    entityType getEntityType()const{return m_entityType;}
    void setEntityType(entityType newType){m_entityType = newType;}

    ngl::Vec3 getPos()const{return m_pos;}
    void setPos(ngl::Vec3 newPos) {m_pos = newPos;}

    float getBoundingRadius()const{return m_boundingRadius;}
    void setBoudingRadius(float r) {m_boundingRadius = r;}

    virtual bool handleMessage(const Message& _message) = 0;

};

#endif // BASEGAMEENTITY_H
