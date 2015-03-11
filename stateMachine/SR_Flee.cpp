#include "SR_Flee.h"

SR_Flee::SR_Flee()
{
}

SR_Flee *SR_Flee::Instance()
{
  static SR_Flee m_instance;
  return &m_instance;
}

void SR_Flee::enter(Rioter* _rioter)
{
    std::cout<<"enter SR_Flee"<<std::endl;
}

void SR_Flee::execute(Rioter* _rioter)
{
    std::cout<<"execute SR_Flee"<<std::endl;
}

void SR_Flee::exit(Rioter* _rioter)
{
    std::cout<<"exit SR_Flee"<<std::endl;
}
