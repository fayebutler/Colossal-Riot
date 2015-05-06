#include "include/MovingEntity.h"



MovingEntity::MovingEntity()
{

}

bool MovingEntity::handleMessage(const Message& _message)
{
  return BaseGameEntity::handleMessage(_message);
}
