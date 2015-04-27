#include "Police.h"

#ifndef SQUAD_H
#define SQUAD_H

class Squad
{
public:
    Squad(GameWorld *world, int squadSize, ngl::Vec3 pos);
    ~Squad(){}

    void update(double timeElapsed, double currentTime);
    void draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX);
    void loadMatricesToShader(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX);

    int getSquadSize() const { return m_squadSize; }
    void setSquadSize(int _val) { m_squadSize = _val; }

    float getSquadRadius() const { return m_squadRadius; }
    void setSquadRadius(float _val) { m_squadRadius = _val; }

    ngl::Vec3 getSquadPos() const { return m_squadPos; }
    void setSquadPos(ngl::Vec3 _val) { m_squadPos = _val; }

private:

    int m_squadSize;
    float m_squadRadius;
    ngl::Vec3 m_squadPos;

    std::vector <Police*> m_squadPolice;
};

#endif // SQUAD_H
