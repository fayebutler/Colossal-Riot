#include "Rioter.h"

Rioter::Rioter(int _ID) : BaseEntity(_ID)
{
    //BaseEntity(_ID);
    m_stateMachine = new StateMachine<Rioter>(this);
    //m_stateMachine->setCurrentState(SR_Protest::Instance());
    m_stateMachine->setCurrentState("protest");
    m_stateMachine->setPreviousState("NULL");
//    m_stateMachine->setGlobalState(Class::Instance());
    m_morale = 100;
}

Rioter::~Rioter()
{
  delete m_stateMachine;
}

void Rioter::update()
{
    m_stateMachine->update();
}
