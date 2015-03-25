#include <iostream>
#include "Rioter.h"
#include "Police.h"
#include "StateMachine.h"
#include "MessageManager.h"


int main()
{
    std::cout<<"Hello World! From C++"<<std::endl;

    Rioter test_rioter;
    Police test_police;

    for (int i = 0; i < 50; ++i)
    {
      test_rioter.update();
      test_police.update();
    }

    //MessageMgr->sendMessage(test_police.getID(),test_rioter.getID(),msgMoraleDown,0,test_police.getRage());

    return 0;
}

