#ifndef SR_ATTACK_H
#define SR_ATTACK_H

#include "State.h"

class Rioter;

class SR_Attack : public State<Rioter>
{
public:
  static SR_Attack* Instance();

  virtual void enter(Rioter* _rioter);
  virtual void execute(Rioter* _rioter);
  virtual void exit(Rioter* _rioter);

private:
  SR_Attack();
  SR_Attack(SR_Attack const&);
  void operator=(SR_Attack const&);

};

#endif // SR_ATTACK_H
