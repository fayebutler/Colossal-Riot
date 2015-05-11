#ifndef SQUAD_H
#define SQUAD_H

//----------------------------------------------------------------------------------------------------------------------------
/// @file Squad.h
/// @brief defines a squad of police
//----------------------------------------------------------------------------------------------------------------------------

#include "Police.h"

//----------------------------------------------------------------------------------------------------------------------
/// @class Squad "include/Squad.h"
/// @brief The class to define a squad of policemen
/// @author Faye Butler, Will Herbert
/// @version 1.0
/// @date Last revision 10/05/2015 Updated to comply with doxygen and NCCA coding standard
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------
/// @brief enum of the different squad states
//----------------------------------------------------------------------------------------------------------------------------
enum eSquadState
{
  squadPatrol,      //0
  squadAggressive,  //1
  squadDefensive,   //2
  squadWall,        //3
  squadMove         //4
};

class Squad : public Vehicle
{
public:
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Squad ctor
  /// @param[in] world: Pointer to the gameworld
  /// @param[in] squadSize: number of policemen in the squad
  /// @param[in] pos: inital position of the squad
  /// @param[in] r: radius of the squad
  /// @param[in] _mesh: pointer to the obj mesh used for a policeman
  //----------------------------------------------------------------------------------------------------------------------------
  Squad(GameWorld *world, int squadSize, ngl::Vec3 pos, float r, ngl::Obj* _mesh);
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Squad dtor
  //----------------------------------------------------------------------------------------------------------------------------
  ~Squad();

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to update the squad every tick
  //----------------------------------------------------------------------------------------------------------------------------
  void update(double timeElapsed, double currentTime);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to draw the squad
  /// @param[in] cam: the camera used in the scene
  /// @param[in] mouseGlobalTX: the global transform of the mouse position
  //----------------------------------------------------------------------------------------------------------------------------
  void draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to load the matrices to the shader
  /// @param[in] cam: the camera used in the scene
  /// @param[in] mouseGlobalTX: the global transform of the mouse position
  //----------------------------------------------------------------------------------------------------------------------------
  void loadMatricesToShader(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief defines virtual function to determine how messages should be handled
  /// @param[in] _messgae:message recieved by the squad
  //----------------------------------------------------------------------------------------------------------------------------
  bool handleMessage(const Message& _message);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Reorders the vector of the walls in order of how close to the squad
  /// @param[in] squad: the squad to measure to the walls from
  //----------------------------------------------------------------------------------------------------------------------------
  void findClosestWalls(Squad* squad);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Creates positions for each of the policeman to form a wall using the closes walls vector
  //----------------------------------------------------------------------------------------------------------------------------
  void formWall();

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to check if the policeman in the squad has died
  /// @return Int: the number of deaths in the squad
  //----------------------------------------------------------------------------------------------------------------------------
  int checkDeaths();

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to check if the colour selected matches the colour of a squad
  /// @param[in] cam: colour: vector to hold the colour selected
  /// @return Boolean: returns true if the colour matches a squad, false if it doesn't
  //----------------------------------------------------------------------------------------------------------------------------
  bool checkSelectionColour(const ngl::Vec3 colour);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to draw the squads with the selection colours
  ///         Not loaded to the shader, just used for selection
  /// @param[in] cam: the camera used in the scene
  /// @param[in] mouseGlobalTX: the global transform of the mouse position
  //----------------------------------------------------------------------------------------------------------------------------
  void selectionDraw(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the squad size
  /// @returns Int: the number of policeman in the squad
  //----------------------------------------------------------------------------------------------------------------------------
  int getSquadSize() const { return m_squadSize; }

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the squad size
  /// @param[in] _val: number to set how many policeman in the squad
  //----------------------------------------------------------------------------------------------------------------------------
  void setSquadSize(const int _val) { m_squadSize = _val; }

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the radius of the squad
  /// @return Float: the size of the radius of the squad
  //----------------------------------------------------------------------------------------------------------------------------
  float getSquadRadius() const { return m_squadRadius; }

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the radius of the squad
  /// @param[in] _val: the number to set the radius of the squad
  //----------------------------------------------------------------------------------------------------------------------------
  void setSquadRadius(const float _val) { m_squadRadius = _val; }

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the colour of the squad
  /// @return Colour: vector of the colour of the squad
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Colour getSquadColour() const { return m_squadColour; }
  ngl::Colour getSquadSelectedColour() const { return m_squadSelectedColour; }

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the colour of the squad
  /// @param[in] _val: the vector to set the colour of the squad to
  //----------------------------------------------------------------------------------------------------------------------------
  void setSquadColour(const ngl::Colour _val) { m_squadColour = _val; }
  void setSquadSelectedColour(const ngl::Colour _val) { m_squadSelectedColour = _val; }
  void setSquadDrawColour(const ngl::Colour _val) { m_squadDrawColour = _val; }

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the police in the squad
  /// @return Vector: returns a vector that contains the police entities in the squad
  //----------------------------------------------------------------------------------------------------------------------------
  std::vector<Police*> getSquadPolice() const {return m_squadPolice;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the target for the squad and police
  /// @param[in] _target: vector position of the target
  //----------------------------------------------------------------------------------------------------------------------------
  void setTarget(const ngl::Vec3 _target);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the target for the squad
  /// @return Vec3: returns the target position
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getTarget() const {return m_target;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the state of the squad
  /// @param[in] _luaState: the lua state to change to
  /// @param[in] _enumState: the enum squad state
  //----------------------------------------------------------------------------------------------------------------------------
  void setSquadState(const char* _luaState, eSquadState _enumState);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the state of the squad
  /// @return eSquadSate: returns the enum squad state
  //----------------------------------------------------------------------------------------------------------------------------
  eSquadState getSquadState() { return m_squadState; }

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to calculate the average police position
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 averagePolicePos();

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the previous state of the squad
  /// @return eSquadSate: returns the previous enum squad state
  //----------------------------------------------------------------------------------------------------------------------------
  void setPreviousState(eSquadState _state){m_previousState = _state;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the previous state of the squad
  /// @return eSquadSate: returns the previous enum squad state
  //----------------------------------------------------------------------------------------------------------------------------
  eSquadState getPreviousState(){return m_previousState;}


private:
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Number of policemen in the squad
  //----------------------------------------------------------------------------------------------------------------------------
  int m_squadSize;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief The radius of the squad
  //----------------------------------------------------------------------------------------------------------------------------
  float m_squadRadius;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Pointer to the obj mesh of a policeman
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Obj* m_mesh;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Vector for the selection colour
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_selectionColour;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Vector for the next selection colour
  //----------------------------------------------------------------------------------------------------------------------------
  static ngl::Vec3 s_nextSelectionColour;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Vector to hold the police entities in the squad
  //----------------------------------------------------------------------------------------------------------------------------
  std::vector <Police*> m_squadPolice;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Vector for the squad colour
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Colour m_squadColour;
  ngl::Colour m_squadSelectedColour;
  ngl::Colour m_squadDrawColour;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Vector for the target of the squad
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_target;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Vector to hold the dead police entities in the squad
  //----------------------------------------------------------------------------------------------------------------------------
  std::vector <Police*> m_deadSquadPolice;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Vector to hold a boolean of whether a policeman has arrived at the target
  //----------------------------------------------------------------------------------------------------------------------------
  std::vector <bool> m_policeArrived;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Boolean to say whether all the policemen have arrived
  //----------------------------------------------------------------------------------------------------------------------------
  bool m_allArrived;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Boolean to say whether the blockade positions have been generated
  //----------------------------------------------------------------------------------------------------------------------------
  bool m_generatedBlockade;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Boolean to say whether the policeman are in the blockade
  //----------------------------------------------------------------------------------------------------------------------------
  bool m_inBlockade;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Vector to hold the blockade position for each policeman
  //----------------------------------------------------------------------------------------------------------------------------
  std::vector<ngl::Vec3> m_blockadePositions;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Vector to contain the walls, in order of how close they are to the squad
  //----------------------------------------------------------------------------------------------------------------------------
  std::vector<Wall> m_closestWalls;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief enum Squad State to hold what the current state of the squad is
  //----------------------------------------------------------------------------------------------------------------------------
  eSquadState m_squadState;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief enum Squad State to hold what the previous state of the squad is
  //----------------------------------------------------------------------------------------------------------------------------
  eSquadState m_previousState;
};

#endif // SQUAD_H
