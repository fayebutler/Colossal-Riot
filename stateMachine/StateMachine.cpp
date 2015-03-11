//#include "StateMachine.h"
//#include "assert.h"

//template<> StateMachine<class entityType>::StateMachine(entityType* _owner)
//{
//    m_owner = _owner;
////    m_currentState = NULL;
////    m_previousState = NULL;
////    m_globalState = NULL;
//}

//template<> void StateMachine<class entityType>::update()
//{
//    if (m_globalState)
//    {
//      m_globalState->execute(m_owner);
//    }
//    if (m_currentState)
//    {
//      m_currentState->execute(m_owner);
//    }
//}

//template<> void StateMachine<class entityType>::changeState(State<entityType>* _newState)
//{
//  assert(_newState && "StateMachine::changeState is trying to change to a null state");
//  m_previousState = m_currentState;
//  m_currentState->exit(m_owner);
//  m_currentState = _newState;
//  m_currentState->enter(m_owner);
//}

//template<> void StateMachine<class entityType>::toPreviousState()
//{
//  changeState(m_previousState);
//}
