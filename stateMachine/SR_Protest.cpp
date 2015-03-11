#include "SR_Flee.h"
#include "SR_Protest.h"
#include "Rioter.h"


SR_Protest::SR_Protest()
{
}

SR_Protest *SR_Protest::Instance()
{
  static SR_Protest m_instance;
  return &m_instance;
}

void SR_Protest::enter(Rioter* _rioter)
{
    std::cout<<"enter SR_Protest"<<std::endl;
}

void SR_Protest::execute(Rioter* _rioter)
{
    std::cout<<"execute SR_Protest"<<std::endl;
    std::cout<<"morale = "<<_rioter->getMorale()<<std::endl;

    if (_rioter->getMorale() < 10)
    {
        std::cout<<":("<<std::endl;
        _rioter->getStateMachine()->changeState(SR_Flee::Instance());
    }
}

void SR_Protest::exit(Rioter* _rioter)
{
    std::cout<<"exit SR_Protest"<<std::endl;
}
