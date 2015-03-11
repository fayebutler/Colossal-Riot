#ifndef SR_FLEE_H
#define SR_FLEE_H

#include "State.h"

class Rioter;

class SR_Flee : public State<Rioter>
{
public:
  static SR_Flee* Instance();

  virtual void enter(Rioter* _rioter);
  virtual void execute(Rioter* _rioter);
  virtual void exit(Rioter* _rioter);

private:
  SR_Flee();
  SR_Flee(SR_Flee const&);
  void operator=(SR_Flee const&);

};

#endif // SR_FLEE_H
