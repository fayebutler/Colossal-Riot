#ifndef RIOTER_H
#define RIOTER_H

#include "BaseEntity.h"
#include "StateMachine.h"
#include "SR_Protest.h"

class Rioter : public BaseEntity
{
public:
    Rioter(int _ID);
    ~Rioter();

    void update();
    StateMachine<Rioter>* getStateMachine() const { return m_stateMachine; }
 // void changeState(State* _newState);


protected:
    StateMachine<Rioter>* m_stateMachine;




};

#endif // RIOTER_H
