
//----------------------------------------------------------------------------------------------------------------------------
/// @file Squad.cpp
/// @brief defines a squad
//----------------------------------------------------------------------------------------------------------------------------

#include <boost/foreach.hpp>
#include "Squad.h"
#include "GameWorld.h"

//----------------------------------------------------------------------------------------------------------------------------

ngl::Vec3 Squad::s_nextSelectionColour = ngl::Vec3(0.0,0.0,0.0);

//----------------------------------------------------------------------------------------------------------------------------
Squad::Squad(GameWorld* world, int squadSize, ngl::Vec3 pos, float r, ngl::Obj *_mesh):Vehicle(world, pos, ngl::Vec3(0,0,0), 0.0f, 1.0f, 10.0f,1.0f, 1.0f, 0.5f)
{  
  m_allArrived = false;
  m_inBlockade = false;
  m_generatedBlockade = false;
  m_squadSize = squadSize;

  m_previousState = squadPatrol;

  m_squadColour = ngl::Colour(0.0f,0.5f,0.5f,1.0f);

  m_squadRadius = squadSize*m_boundingRadius;
  m_mesh = _mesh;

  for (int i = 0; i < squadSize; ++i)
  {
    Police* newPolice = new Police(world, m_mesh);
    newPolice->setBoudingRadius(m_boundingRadius);
    newPolice->setDetectionRadius(3.5f);
    newPolice->setPos(ngl::Vec3((((float)rand()/RAND_MAX)*m_squadRadius*2)-m_squadRadius+ m_pos.m_x, 0.0f, (((float)rand()/RAND_MAX)*m_squadRadius*2)-m_squadRadius+ m_pos.m_z));
    world->getCellGraph()->initializeCells(m_entityMgr->getEntityFromID(newPolice->getID()));
    while (newPolice->getCurrentCellID() < 0)
    {
      newPolice->setPos(ngl::Vec3((((float)rand()/RAND_MAX)*m_squadRadius*2)-m_squadRadius+ m_pos.m_x, 0.0f, (((float)rand()/RAND_MAX)*m_squadRadius*2)-m_squadRadius+ m_pos.m_z));
      world->getCellGraph()->initializeCells(m_entityMgr->getEntityFromID(newPolice->getID()));
    }
    newPolice->setSquadPos(m_pos);
    newPolice->setSquadRadius(m_squadRadius);
    newPolice->setSquadID(m_ID);
    m_squadPolice.push_back(newPolice);
    m_policeArrived.push_back(false);
  }
  setSquadState("patrol", squadPatrol);
  m_selectionColour = s_nextSelectionColour;

  s_nextSelectionColour.m_x += 0.1;

  if(s_nextSelectionColour.m_x >= 1)
  {
    s_nextSelectionColour.m_x = 0;
    s_nextSelectionColour.m_y += 0.1;

    if(s_nextSelectionColour.m_y >= 1)
    {
      s_nextSelectionColour.m_y = 0;
      s_nextSelectionColour.m_z += 0.1;
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------------
Squad::~Squad()
{
  BOOST_FOREACH(Police *p, m_squadPolice)
  {
      delete p;
  }
  m_squadPolice.clear();
  BOOST_FOREACH(Police *p, m_deadSquadPolice)
  {
      delete p;
  }
  m_deadSquadPolice.clear();
}

//----------------------------------------------------------------------------------------------------------------------------
/// Average Police Pos - function to calculate the average position of the police in the squad
//----------------------------------------------------------------------------------------------------------------------------
ngl::Vec3 Squad::averagePolicePos()
{
  ngl::Vec3 currentTotal =0;
  for (int i = 0; i< m_squadSize; i++)
  {
    Police* currentPolice = m_squadPolice[i];
    currentTotal += currentPolice->getPos();
  }
   currentTotal /= m_squadSize;
  return currentTotal;
}

//----------------------------------------------------------------------------------------------------------------------------
/// Update - updates the police within the squad, according to what state they are in
///           if in state move, then police are set to moving (lua state)
///           if in state wall, form wall is called and crosshairs are set to individual blockade positions
//----------------------------------------------------------------------------------------------------------------------------
void Squad::update(double timeElapsed, double currentTime)
{
  // squad checks

  // SQUAD MOVE
  if(m_squadState == squadMove )
  {
    m_pos = averagePolicePos();

    // check if all police have arrived- break if one hasn't

    m_allArrived = true;

    for(unsigned int i=0; i<m_squadSize; ++i)
    {
       Police* currentPolice = m_squadPolice[i];
       currentPolice->setIsMoving(false);
       if(m_policeArrived[i]==false)
       {
         //set to moving, sets the police to lua state move
         m_allArrived = false;
         currentPolice->setIsMoving(true);
         break;
       }
     }
    //switch out of move if police have ALL arrived
    if(m_allArrived == true)
    {
      m_squadState = m_previousState;
    }
  }

  // SQUAD WALL
  else if(m_squadState == squadWall)
  {
    this->formWall();
  }


  // individual police loop
  for(unsigned int i=0; i<m_squadSize; ++i)
  {
    Police* currentPolice = m_squadPolice[i];
    currentPolice->setSquadPos(m_pos);
    currentPolice->setSquadRadius(m_squadRadius);

    // MOVE
    if(m_squadState == squadMove)
    {
      if ((currentPolice->getPos() - m_target).lengthSquared()<= 4)
      {
        m_policeArrived[i] = true;
        currentPolice->setIsMoving(false);
      }
    }
    else
    {
      //WALL
      if (m_generatedBlockade == false)
      {
        currentPolice->setCrosshair(m_pos);
      }
      currentPolice->setIsMoving(false);
    }

    // WALL
    if(m_squadState == squadWall && m_generatedBlockade == true)
    {
      currentPolice->setBlockadePos(m_blockadePositions[i]);
    }
    else if(m_squadState != squadWall)
    {
      currentPolice->setBlockadePos(NULL);
    }
    //finally update the police
    currentPolice->update(timeElapsed, currentTime);
  }
}

//----------------------------------------------------------------------------------------------------------------------------
/// Draw - calls the police draw and loads squad draws matrices to shader
//----------------------------------------------------------------------------------------------------------------------------
void Squad::draw(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{

    for(unsigned int i=0; i<m_squadSize; ++i)
    {
      Police* currentPolice = m_squadPolice[i];
      currentPolice->draw(cam, mouseGlobalTX);
    }

  loadMatricesToShader(cam, mouseGlobalTX);
  ngl::VAOPrimitives::instance()->createDisk("squad",m_squadRadius,12);
  ngl::VAOPrimitives::instance()->draw("squad");
}

//----------------------------------------------------------------------------------------------------------------------------
/// Draw Target - draws the target in the move state
//----------------------------------------------------------------------------------------------------------------------------
void Squad::drawTarget(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{
  if(m_squadState == squadMove)
  {
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["Phong"]->use();
    ngl::Material m(ngl::Colour(0.2f,0.2f,0.2f, 1.0), ngl::Colour(0.2775f,0.2775f,0.2775f, 1.0), ngl::Colour(0.77391f,0.77391f,0.77391f, 1.0));
    m.setSpecularExponent(5.f);
    m.setDiffuse(ngl::Colour(m_squadDrawColour.m_r+0.3f,m_squadDrawColour.m_g+0.3f,m_squadDrawColour.m_b+0.3f,m_squadDrawColour.m_a));
    m.loadToShader("material");

      ngl::Mat4 MV;
      ngl::Mat4 MVP;
      ngl::Mat3 normalMatrix;
      ngl::Mat4 M;
      ngl::Transformation trans;
      trans.setPosition(m_target.m_x, 0.005f, m_target.m_z);
      trans.setRotation(90.0,0.0,0.0);

    M=trans.getMatrix()*mouseGlobalTX;
    MV=  M*cam->getViewMatrix();
    MVP= M*cam->getVPMatrix();
    normalMatrix=MV;
    normalMatrix.inverse();

      shader->setShaderParamFromMat4("MVP",MVP);
      shader->setShaderParamFromMat3("normalMatrix",normalMatrix);




    ngl::VAOPrimitives::instance()->createDisk("target",1.0,12);
    ngl::VAOPrimitives::instance()->draw("target");
  }
}

//----------------------------------------------------------------------------------------------------------------------------
void Squad::loadMatricesToShader(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["Phong"]->use();

  ngl::Material m(ngl::Colour(0.2f,0.2f,0.2f, 1.0), ngl::Colour(0.2775f,0.2775f,0.2775f, 1.0), ngl::Colour(0.77391f,0.77391f,0.77391f, 1.0));
  m.setSpecularExponent(5.f);
  m.setDiffuse(ngl::Colour(m_squadColour));
  m.loadToShader("material");

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  ngl::Transformation trans;
  trans.setPosition(m_pos.m_x, 0.004, m_pos.m_z);
  trans.setRotation(90.0,15.0,0.0);


  M=trans.getMatrix()*mouseGlobalTX;
  MV=  M*cam->getViewMatrix();
  MVP= M*cam->getVPMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);

}

//----------------------------------------------------------------------------------------------------------------------------
/// Check Selection colour - checks if the selected colour matches the colour of a squad
///                         - returns true if it does, false if it doesn't
//----------------------------------------------------------------------------------------------------------------------------
bool Squad::checkSelectionColour(const ngl::Vec3 colour)
{
  if(m_selectionColour.m_x >= colour.m_x-0.01f && m_selectionColour.m_x <= colour.m_x+0.01f &&
     m_selectionColour.m_y >= colour.m_y-0.01f && m_selectionColour.m_y <= colour.m_y+0.01f &&
     m_selectionColour.m_z >= colour.m_z-0.01f && m_selectionColour.m_z <= colour.m_z+0.01f)
  {
    return true;
  }
  else
  {
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------
/// Selection Draw - this draws the squad colours as used for the colour selection (this is not actually loaded to the shader)
//----------------------------------------------------------------------------------------------------------------------------
void Squad::selectionDraw(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglColourShader"]->use();
  shader->setShaderParam4f("Colour", m_selectionColour.m_x, m_selectionColour.m_y,m_selectionColour.m_z, 1);

  ngl::Transformation t;
  t.setPosition(m_pos.m_x, 0.3, m_pos.m_z);
  t.setRotation(90.0,0.0,0.0);

  ngl::Mat4 M;
  ngl::Mat4 MV;
  ngl::Mat4 MVP;

  M=t.getMatrix()*mouseGlobalTX;
  MV=  M*cam->getViewMatrix();
  MVP= M*cam->getVPMatrix();

  shader->setShaderParamFromMat4("MVP",MVP);

  ngl::VAOPrimitives::instance()->createDisk("squad",m_squadRadius,120);
  ngl::VAOPrimitives::instance()->draw("squad");

}

//----------------------------------------------------------------------------------------------------------------------------
/// Set Target - sets the previous state of the squad when it enters move, and finds a path to the target
//----------------------------------------------------------------------------------------------------------------------------
void Squad::setTarget(const ngl::Vec3 _target)
{
  if(m_squadState != squadMove)
  {
    m_previousState = m_squadState;
  }

  m_target = _target;
  for(unsigned int i=0; i<m_squadSize; ++i)
  {
    Police* currentPolice = m_squadPolice[i];
    currentPolice->setSquadPos(m_pos);
    currentPolice->setSquadRadius(m_squadRadius);
    currentPolice->findPath(_target);
    currentPolice->setIsMoving(true);
    m_policeArrived[i] = false;
  }
  m_allArrived = false;
  m_inBlockade = false;
  m_squadState = squadMove;
}

//----------------------------------------------------------------------------------------------------------------------------
/// Check Deaths - function to check the number of police in the squad that are dead
//----------------------------------------------------------------------------------------------------------------------------
int Squad::checkDeaths()
{
  int numberOfDeaths = 0;
  for(int i=0; i<m_squadSize; i++)
  {
    Police* currentPolice = m_squadPolice[i];
    if(currentPolice->getHealth()<=0)
    {
        currentPolice->death();
        m_entityMgr->removeEntity(dynamic_cast<BaseGameEntity*>(currentPolice));
        m_deadSquadPolice.push_back(currentPolice);
        m_squadPolice.erase(m_squadPolice.begin()+i);
        m_squadSize -= 1;
        numberOfDeaths++;
        i--;
    }
  }
  return numberOfDeaths;
}

//----------------------------------------------------------------------------------------------------------------------------
bool Squad::handleMessage(const Message& _message)
{
  switch(_message.m_message)
  {
  case msgAttack:

    return true;
    break;

  default:
    std::cout<<"Squad: Message type not defined"<<std::endl;
    return false;
    break;
  }
}

//----------------------------------------------------------------------------------------------------------------------------
/// Closest Walls - this iterates through a vector of the closest walls to the squad and
///                 reorders it with the closest first, to be used in the blockade
//----------------------------------------------------------------------------------------------------------------------------
void Squad::findClosestWalls(Squad* squad)
{
  m_closestWalls.clear();
  std::vector<Wall> inWalls = squad->getCurrentCell().getWalls();
  std::vector<int> wallMemory;
  int numberOfWallsToCheck = inWalls.size();

  while (m_closestWalls.size() < numberOfWallsToCheck)
  {
    float shortestDist;
    Wall closestWall;
    int closestWallID = 0;
    bool isInMemory = false;
    for (int i = 0; i < numberOfWallsToCheck; i++ )
    {
      shortestDist = 10000000000.0f;
      isInMemory = false;
      for (std::vector<int>::iterator iter = wallMemory.begin(); iter != wallMemory.end(); ++iter)
      {

        if( *iter == i)
        {
          isInMemory = true;
          break;
        }

      }
      if(isInMemory == true){break;}
      Wall currentWall = inWalls[i];
      ngl::Vec3 currentCenter = (currentWall.start+currentWall.end)/2;

      ngl::Vec3 distance = (currentCenter-m_pos);

      if(distance.lengthSquared() < shortestDist)
      {
        shortestDist = distance.lengthSquared();
        closestWall = inWalls[i];
        closestWallID = i;
      }
    }
    wallMemory.push_back(closestWallID);
    m_closestWalls.push_back(closestWall);

  }
  inWalls.clear();
  wallMemory.clear();

}

//----------------------------------------------------------------------------------------------------------------------------
/// Form wall - used to form a blockade of policemen, taking the closest walls and cells it computes the positions
///              based on how many police in the squad and their bounding radius
//----------------------------------------------------------------------------------------------------------------------------
void Squad::formWall()
{

    m_inBlockade = false;
    int numberOfWallsToCheck =0;
    m_generatedBlockade = false;
    //check to make sure not already made
    if(m_generatedBlockade == false)
    {
      //update the closest walls vector
      findClosestWalls(this);
      numberOfWallsToCheck = m_closestWalls.size();
    }

    m_blockadePositions.clear();
    for(int i=0; i<numberOfWallsToCheck; i++)
    {
      if(m_generatedBlockade == false)
      {
          Wall currentWall = m_closestWalls[i];
          ngl::Vec3 currentNormal = currentWall.normal;
          ngl::Vec3 currentCenter = (currentWall.start+currentWall.end)/2;
          for(int j=0; j<numberOfWallsToCheck; j++)
          {
            //check not testing same wall against itself
            if(i!=j)
            {
              Wall testWall = m_closestWalls[j];
              ngl::Vec3 testNormal = testWall.normal;
              ngl::Vec3 testCenter = (testWall.start+testWall.end)/2;

              // if true this means two walls are facing each other
              if(currentNormal + testNormal == ngl::Vec3(0.0,0.0,0.0))
              {
                //if vertical line
                if(currentCenter.m_x - testCenter.m_x == 0)
                {
                  ngl::Vec3 distance = currentCenter - testCenter;
                  ngl::Vec3 upperCenter = ngl::Vec3(0.0,0.0,0.0);

                  //test which wall is the higher wall
                  if(currentCenter.m_z < testCenter.m_z)
                  {
                    upperCenter = currentCenter;
                  }
                  else if(currentCenter.m_z > testCenter.m_z)
                  {
                    upperCenter = testCenter;
                  }
                  float dist = distance.length();
                  float numberOf= dist/m_boundingRadius;

                  if(numberOf >= m_squadSize)
                  {
                    //calculate spacing for however many police in squad
                    float spacing = dist/(m_squadSize*2);
                    m_blockadePositions.clear();
                    for(int i =0; i< m_squadSize*2; i+=2)
                    {
                      //set position, the x value should be the middle of the squad as it is value that doesn't change
                      ngl::Vec3 position = ngl::Vec3(m_pos.m_x, 0.0, upperCenter.m_z +((i+1)*spacing));
                      m_blockadePositions.push_back(position);
                    }

                   }
                  m_generatedBlockade = true;
                  m_inBlockade = true;
                   break;

                  }

                else if(currentCenter.m_z - testCenter.m_z == 0)
                {
                  //if horizontal line
                  ngl::Vec3 distance = currentCenter - testCenter;
                  ngl::Vec3 leftCenter = ngl::Vec3(0.0,0.0,0.0);

                  //test which wall is to the left
                  if(currentCenter.m_x < testCenter.m_x)
                  {
                    leftCenter = currentCenter;
                  }
                  else if(currentCenter.m_x > testCenter.m_x)
                  {
                    leftCenter = testCenter;
                  }
                  float dist = distance.length();
                  float numberOf= dist/m_boundingRadius;

                  if(numberOf >= m_squadSize)
                  {
                    //calculate spacing for however many police in squad
                    float spacing = dist/(m_squadSize*2);

                    m_blockadePositions.clear();
                    for(int i =0; i< m_squadSize*2; i+=2)
                    {
                      //set position, the x value should be the middle of the squad as it is value that doesn't change
                      ngl::Vec3 position = ngl::Vec3(leftCenter.m_x +((i+1)*spacing), 0.0, m_pos.m_z);
                      m_blockadePositions.push_back(position);
                    }
                   }
                  m_generatedBlockade = true;
                  m_inBlockade = true;
                  break;

                }
                else
                {
                  m_generatedBlockade = false;
                  m_inBlockade = false;

                  std::cout<<" CANT FORM WALL "<<std::endl;
                }


               }
              else
              {
                m_generatedBlockade = false;
                m_inBlockade = false;

               std::cout<<" CANT FORM WALL"<<std::endl;
              }

            }
          }
        }
      }
}

//----------------------------------------------------------------------------------------------------------------------------
/// Set Squad State - used to change the squad state to the eSquadState in lua
//----------------------------------------------------------------------------------------------------------------------------
void Squad::setSquadState(const char *_luaState, eSquadState _enumState)
{
  m_squadState = _enumState;
  for(unsigned int i=0; i<m_squadSize; ++i)
  {
    Police* currentPolice = m_squadPolice[i];
    currentPolice->getStateMachine()->changeState(_luaState);
  }
}





