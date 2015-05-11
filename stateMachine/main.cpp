#include <iostream>
#include "Rioter.h"
#include "Police.h"
#include "StateMachine.h"
#include "MessageManager.h"


int main()
{
    Rioter test_rioter;
    Police test_police;


    for (int i = 0; i < 50; ++i)
    {
      std::cout<<"------------- TICK "<<i<<" -------------"<<std::endl;
      test_rioter.update();
      test_police.update();
      std::cout<<"Rioter: health = "<<test_rioter.getHealth()<<", morale = "<<test_rioter.getMorale()<<std::endl;
      std::cout<<"Police: health = "<<test_police.getHealth()<<", morale = "<<test_police.getMorale()<<std::endl;
      std::cout<<"\n"<<std::endl;
    }

    return 0;
}

