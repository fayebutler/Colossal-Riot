#ifndef SR_PROTEST_H
#define SR_PROTEST_H

#include "State.h"

class Rioter;

class SR_Protest : public State<Rioter>
{
public:
  static SR_Protest* Instance();

  virtual void enter(Rioter* _rioter);
  virtual void execute(Rioter* _rioter);
  virtual void exit(Rioter* _rioter);

private:
  SR_Protest();
  SR_Protest(SR_Protest const&);
  void operator=(SR_Protest const&);

};

#endif // SR_PROTEST_H
