#ifndef STATICENTITY_H
#define STATICENTITY_H

//----------------------------------------------------------------------------------------------------------------------------
/// @file StaticEntity.h
/// @brief defines a static entity
//----------------------------------------------------------------------------------------------------------------------------

#include "BaseGameEntity.h"

#include <ngl/ShaderLib.h>
#include <ngl/NGLInit.h>
#include <ngl/Material.h>
#include <ngl/Transformation.h>
#include <ngl/Camera.h>
#include <ngl/Obj.h>

//----------------------------------------------------------------------------------------------------------------------
/// @class StaticEntity "include/StaticEntity.h"
/// @brief The base class for all static entities
/// @author Emily Holt & Faye Butler
/// @version 1.0
/// @date Last revision 10/05/2015 Updated to comply with doxygen and NCCA coding standard
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------
/// @brief enum of the different obstacle types
//----------------------------------------------------------------------------------------------------------------------------
enum obstacleType
{
    obstacleTree,       //0
    obstacleStreetLight //1
};

class StaticEntity : public BaseGameEntity
{
public:
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Static Entity ctor
  /// @param[in] world: pointer to the game world
  /// @param[in] pos: vector position of the entity
  /// @param[in] dir: rotation of the entity in degrees
  /// @param[in] r: radius of the entity
  /// @param[in] _type: the type of obstacle the entity is
  /// @param[in] _mesh: pointer to the obj mesh of the entity
  //----------------------------------------------------------------------------------------------------------------------------
  StaticEntity(GameWorld *world, ngl::Vec3 pos, ngl::Vec3 dir, float r, obstacleType _type, ngl::Obj *_mesh);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief StaticEntity dtor
  //----------------------------------------------------------------------------------------------------------------------------
  ~StaticEntity(){}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to draw the obstacle entities
  /// @param[in] cam: the camera used in the scene
  /// @param[in] mouseGlobalTX: the global mouse position transform matrix
  //----------------------------------------------------------------------------------------------------------------------------
  void draw(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to load the matrices of the entity to the shader
  /// @param[in] cam: the camera used in the scene
  /// @param[in] mouseGlobalTX: the global mouse position transform matrix
  //----------------------------------------------------------------------------------------------------------------------------
  void loadMatricesToShader(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to define how to handle the messages
  /// @param[in] _message: the message passed to the entity
  //----------------------------------------------------------------------------------------------------------------------------
  virtual bool handleMessage(const Message& _message);

private:
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief pointer to the obj mesh for the entity
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Obj *m_mesh;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief defines the obstacle type
  //----------------------------------------------------------------------------------------------------------------------------
  obstacleType m_obstacleType;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief vector of the entities rotation
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_dir;

};

#endif // STATICENTITY_H
