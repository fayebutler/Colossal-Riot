#ifndef STATICENTITY_H
#define STATICENTITY_H

#include "BaseGameEntity.h"

#include <ngl/ShaderLib.h>
#include <ngl/NGLInit.h>
#include <ngl/Material.h>
#include <ngl/Transformation.h>
#include <ngl/Camera.h>
#include <ngl/Obj.h>

enum obstacleType
{
    obstacleTree,
    obstacleStreetLight
};

class StaticEntity : public BaseGameEntity
{
public:
    StaticEntity(GameWorld *world, ngl::Vec3 pos, ngl::Vec3 dir, float r, obstacleType _type, ngl::Obj *_mesh);
    ~StaticEntity(){}
    void draw(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX);
    void loadMatricesToShader(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX);
    virtual bool handleMessage(const Message& _message);

private:
    ngl::Obj *m_mesh;
    obstacleType m_obstacleType;

    ngl::Vec3 m_dir;

};

#endif // STATICENTITY_H
