#ifndef POLICE_H
#define POLICE_H

#include "BaseEntity.h"

class Police : public BaseEntity
{
public:
    Police(int _ID);

    void update();

//    int getWeapon() { return m_weapon; }
//    void setWeapon(int _val) { m_weapon = _val; }


protected:
//    int m_weapon();
};

#endif // POLICE_H
