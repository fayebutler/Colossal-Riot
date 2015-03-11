#include <iostream>
#include "Rioter.h"

int main()
{
    std::cout<<"Hello World! From C++"<<std::endl;

    Rioter test_rioter(1);


    for (int i = 0; i < 50; ++i)
    {
      test_rioter.update();
    }

    return 0;
}

