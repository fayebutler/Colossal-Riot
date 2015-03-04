#ifndef BASEENTITY_H
#define BASEENTITY_H

class BaseEntity
{
public:
    BaseEntity(int _ID);
    virtual ~BaseEntity();

    virtual void update() = 0;
    int getID() const { return m_ID; }

    int getHealth() { return m_health; }
    void setHealth(float _val) { m_health = _val; }
    int getEnergy() { return m_energy; }
    void setEnergy(float _val) { m_energy = _val; }
    int getMorale() { return m_morale; }
    void setMorale(float _val) { m_morale = _val; }
    int getRage() { return m_rage; }
    void setRage(float _val) { m_rage = _val; }

protected:
    int m_ID;
    static int m_nextValidID;
    void setID(int _val) { m_ID = _val; }

    float m_health;
    float m_energy;
    float m_morale;
    float m_rage;
};

#endif // BASEENTITY_H
