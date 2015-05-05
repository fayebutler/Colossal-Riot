#include "include/MovingEntity.h"



MovingEntity::MovingEntity()
{

}

bool MovingEntity::handleMessage(const Message& _message)
{
  return BaseGameEntity::handleMessage(_message);
}

//void MovingEntity::stopOverlap(BaseGameEntity* _selfEntity, std::vector<BaseGameEntity *> _otherEntities)
//{
//  for (int i = 0; 9 < _otherEntities.size(); ++i)
//  {
//    BaseGameEntity* currentEntity = _otherEntities[i];
//    if (_selfEntity->getID() != currentEntity->getID())
//    {
//      ngl::Vec3 vectorBetween = _selfEntity->getPos() - currentEntity->getPos();
//      double distanceBetween = vectorBetween.length();
//      double overlapAmount = (_selfEntity->getBoundingRadius() + currentEntity->getBoundingRadius()) - distanceBetween;

//      if (overlapAmount >= 0)
//      {
//        _selfEntity->setPos(_selfEntity->getPos() + (vectorBetween / distanceBetween) * overlapAmount);
//      }
//    }
//  }

//}

//MovingEntity::MovingEntity(ngl::Vec3 pos, float r, ngl::Vec3 velocity, float max_speed, ngl::Vec3 heading, float mass, float turn_rate, float max_force)
//{




//    m_velocity = velocity;
//    m_heading = heading;
//    m_mass = mass;
//    m_side = m_heading.cross(ngl::Vec3(0,1,0));
//    m_maxSpeed = max_speed;
//    m_maxForce = max_force;
//    m_maxTurnRate = turn_rate;
//}
