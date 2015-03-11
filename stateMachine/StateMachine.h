#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "State.h"
#include "assert.h"

template <class entityType>

class StateMachine
{
public:
    StateMachine(entityType* _owner):m_owner(_owner){}

    void setCurrentState(State<entityType>* _inState) { m_currentState = _inState; }
    State<entityType>* getCurrenState() const { return m_currentState; }
    void setPreviousState(State<entityType>* _inState) { m_previousState = _inState; }
    State<entityType>* getPreviousState() const { return m_previousState; }
    void setGlobalState(State<entityType>* _inState) { m_globalState = _inState; }
    State<entityType>* getGlobalState() const { return m_globalState; }

    void update() const
    {
      if (m_globalState)
      {
        m_globalState->execute(m_owner);
      }
      if (m_currentState)
      {
        m_currentState->execute(m_owner);
      }
    }

    void changeState(State<entityType>* _newState)
    {
      assert(_newState && "StateMachine::changeState is trying to change to a null state");
      m_previousState = m_currentState;
      m_currentState->exit(m_owner);
      m_currentState = _newState;
      m_currentState->enter(m_owner);
    }

    void toPreviousState()
    {
      changeState(m_previousState);
    }


private:
    entityType* m_owner;
    State<entityType>* m_currentState;
    State<entityType>* m_previousState;
    State<entityType>* m_globalState;



};

#endif // STATEMACHINE_H
