#include "Police.h"

Police::Police(int _ID) : BaseEntity(_ID)
{

}

void Police::update()
{
    --m_energy;
}

