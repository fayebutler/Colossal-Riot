#include "include/BaseGameEntity.h"

BaseGameEntity::BaseGameEntity()
{
}

BaseGameEntity::BaseGameEntity(int entity_type, ngl::Vec3 pos, float r)
{
    m_pos = pos;
    m_boundingRadius = r;
    m_entityType = entity_type;

}
