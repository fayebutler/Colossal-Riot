#include "SR_Attack.h"

SR_Attack::SR_Attack()
{
}

SR_Attack *SR_Attack::Instance()
{
  static SR_Attack m_instance;
  return &m_instance;
}

void SR_Attack::enter(Rioter* _rioter)
{
    std::cout<<"enter SR_Attack"<<std::endl;
}

void SR_Attack::execute(Rioter* _rioter)
{
    std::cout<<"execute SR_Attack"<<std::endl;
}

void SR_Attack::exit(Rioter* _rioter)
{
    std::cout<<"exit SR_Attack"<<std::endl;
}
