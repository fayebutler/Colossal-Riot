#include "Police.h"
#include "BaseGameEntity.h"

#ifndef SQUAD_H
#define SQUAD_H

class Squad : public BaseGameEntity
{
public:
    Squad(GameWorld *world, int squadSize, ngl::Vec3 pos, float r);
    ~Squad(){}

    void update(double timeElapsed, double currentTime);
    void draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX);
    void loadMatricesToShader(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX);
    bool handleMessage(const Message& _message);

    int getSquadSize() const { return m_squadSize; }
    void setSquadSize(int _val) { m_squadSize = _val; }

    float getSquadRadius() const { return m_squadRadius; }
    void setSquadRadius(float _val) { m_squadRadius = _val; }

//    ngl::Vec3 getSquadPos() const { return m_squadPos; }
//    void setSquadPos(ngl::Vec3 _val) { m_squadPos = _val; }
    bool checkSelectionColour(const ngl::Vec3 colour);
    void selectionDraw(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX);

    ngl::Colour getSquadColour() const { return m_squadColour; }
    void setSquadColour(ngl::Colour _val) { m_squadColour = _val; }

private:

    int m_squadSize;
    float m_squadRadius;
//    ngl::Vec3 m_squadPos;

    ngl::Vec3 m_selectionColour;

    static ngl::Vec3 s_nextSelectionColour;

    std::vector <Police*> m_squadPolice;

    ngl::Colour m_squadColour;

};

#endif // SQUAD_H