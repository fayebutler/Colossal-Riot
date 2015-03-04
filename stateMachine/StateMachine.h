#ifndef STATEMACHINE_H
#define STATEMACHINE_H

template <class entityType>

class StateMachine
{
public:
    StateMachine(entityType* _owner);



private:
    entityType* m_owner;



};

#endif // STATEMACHINE_H
