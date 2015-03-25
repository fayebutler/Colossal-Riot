#ifndef BASEGAMEENTITY_H
#define BASEGAMEENTITY_H

#include <ngl/Transformation.h>


class BaseGameEntity
{
protected:
    int m_ID;
    int m_entityType;
    ngl::Vec3 m_pos;
    float m_boundingRadius;
//    BaseGameEntity();
//    BaseGameEntity(int entity_type, ngl::Vec3 pos, float r);


public:
    BaseGameEntity();
    BaseGameEntity(int entity_type, ngl::Vec3 pos, float r);

 //   ~BaseGameEntity();

    int getEntityType()const{return m_entityType;}
    void setEntityType(int newType){m_entityType = newType;}

    ngl::Vec3 getPos()const{return m_pos;}
    void setPos(ngl::Vec3 newPos) {m_pos = newPos;}

    float getBoundingRadius()const{return m_boundingRadius;}
    void setBoudingRadius(float r) {m_boundingRadius = r;}

};

#endif // BASEGAMEENTITY_H
