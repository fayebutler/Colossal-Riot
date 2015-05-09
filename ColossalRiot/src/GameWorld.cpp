#include "GameWorld.h"

#include <iostream>


GameWorld::GameWorld(int _level)
{
   L = luaL_newstate();
   registerLua(L);

   m_entityMgr = new EntityManager();

   m_win = 0;
   m_lose = 0;

   m_numberOfRiotersDead = 0;

   m_numberOfRiotersHome = 0;

   m_activePolice = 0;

   m_resetID = 1;

   switch (_level)
   {
     case 1 :
     {
       luabridge::LuaRef startLevel = luabridge::getGlobal(L, "level1");
       startLevel();
       break;
     }
     case 2 :
     {
       luabridge::LuaRef startLevel = luabridge::getGlobal(L, "level2");
       startLevel();
       break;
     }
     case 3 :
     {
       luabridge::LuaRef startLevel = luabridge::getGlobal(L, "level3");
       startLevel();
       break;
     }
     case 4 :
     {
       luabridge::LuaRef startLevel = luabridge::getGlobal(L, "level4");
       startLevel();
       break;
     }
     case 5 :
     {
       luabridge::LuaRef startLevel = luabridge::getGlobal(L, "level5");
       startLevel();
       break;
     }
     default :
     {
       std::cout<<"Error: Invalid level"<<std::endl;
       break;
     }
   }

   m_streetMesh = new ngl::Obj(m_streetMeshFile); //Obj for roads, should be tris
   m_buildingMesh = new ngl::Obj(m_buildingMeshFile); //Obj for buildings, should be tris
   m_streetMesh->createVAO();
   m_buildingMesh->createVAO();

   m_cellGraph = new CellGraph(m_cellGraphFile, 1); //Obj for cell graph, must be quads
   m_cellGraph->generateWalls();

   m_policeMesh = new ngl::Obj("policeMan.obj");
   m_rioterMesh = new ngl::Obj("rioterMan.obj");

   m_policeMesh->createVAO();
   m_rioterMesh->createVAO();

  for (int i = 0; i < m_initialNumberOfRioters ; ++i)
  {
    Rioter* newRioter = new Rioter(this, m_rioterMesh);
    newRioter->setBoudingRadius(0.5f);
    newRioter->setDetectionRadius(3.5f);
    newRioter->setHeading(ngl::Vec3(-1+2*((float)rand())/RAND_MAX, 0.f, -1+2*((float)rand())/RAND_MAX));
    newRioter->setPos(ngl::Vec3(-25+50*((float)rand())/RAND_MAX, 0.f, -25+50*((float)rand())/RAND_MAX));
    m_cellGraph->initializeCells(m_entityMgr->getEntityFromID(newRioter->getID()));
    while (newRioter->getCurrentCellID() < 0)
    {
      newRioter->setPos(ngl::Vec3(-50+100*((float)rand())/RAND_MAX, 0.f, -50+100*((float)rand())/RAND_MAX));
      m_cellGraph->initializeCells(m_entityMgr->getEntityFromID(newRioter->getID()));
      while (newRioter->getCurrentCellID() < 0)
      {
        newRioter->setPos(ngl::Vec3(-50+100*((float)rand())/RAND_MAX, 0.f, -50+100*((float)rand())/RAND_MAX));
        m_cellGraph->initializeCells(m_entityMgr->getEntityFromID(newRioter->getID()));
      }
    }
    m_rioters.push_back(newRioter);
  }
  m_numberOfRioters = m_rioters.size();
}

void GameWorld::setPoliceStation(float _x, float _y, float _z)
{
    ngl::Vec3 pos = ngl::Vec3(_x,_y,_z);
    m_policeStation = pos;
}

GameWorld::~GameWorld()
{
   lua_close(L);
   m_rioters.clear();
   m_squads.clear();
   delete m_streetMesh;
   delete m_buildingMesh;
   delete m_entityMgr;
}

void GameWorld::Update(double timeElapsed, double currentTime)
{
    //check for deaths/homes
    m_numberOfSquads = m_squads.size();
    m_numberOfRioters = m_rioters.size();

    for(int i=0; i<m_numberOfRioters; i++)
    {
        Rioter* currentRioter = m_rioters[i];
        std::vector<float> map_bounds = m_cellGraph->getMapBounds();

        //check for rioter deaths
        if(currentRioter->getHealth()<=0.f)
        {
            currentRioter->death();
            m_entityMgr->removeEntity(dynamic_cast<BaseGameEntity*>(currentRioter));

            delete currentRioter;

            m_rioters.erase(m_rioters.begin()+i);
            m_numberOfRioters--;
            m_numberOfRiotersDead ++;
            i--;
        }

        //check for when rioters have left the map
        else if(currentRioter->getPos().m_z <= map_bounds[0] ||
                currentRioter->getPos().m_z >= map_bounds[1] ||
                currentRioter->getPos().m_x <= map_bounds[2] ||
                currentRioter->getPos().m_x >= map_bounds[3])

        {
            m_entityMgr->removeEntity(dynamic_cast<BaseGameEntity*>(currentRioter));
            delete currentRioter;
            m_rioters.erase(m_rioters.begin()+i);
            m_numberOfRioters--;
            m_numberOfRiotersHome++;
            i--;
        }
    }

    //check for squad deaths
    int m_numberOfSquads = m_squads.size();
    for(int i = 0; i < m_numberOfSquads; i++)
    {
      Squad* currentSquad = m_squads[i];

      if (currentSquad->getSquadSize() <= 0)
      {
          m_entityMgr->removeEntity(currentSquad);
          delete m_squads[i];
          m_squads.erase(m_squads.begin()+i);
          m_numberOfSquads--;
          i--;
      }
      else
      {
        m_activePolice -= m_squads[i]->checkDeaths();
      }
    }

    // update cells

    m_cellGraph->clearCells();
    std::map<int,BaseGameEntity*> myMap = m_entityMgr->getEntityMap();

    for (std::map<int,BaseGameEntity*>::const_iterator it = myMap.begin(); it != myMap.end(); ++it)
    {

        //Adds entities to cells and cell ID to entities
        m_cellGraph->updateCells(it->second);
    }

    for (std::map<int,BaseGameEntity*>::const_iterator it = myMap.begin(); it != myMap.end(); ++it)
    {
        //Adds entity neighbours:
      if (it->second->getCurrentCellID() >= 0)
      {
        m_cellGraph->addEntities(it->second);
      }
    }


    // call rioter and squad updates

    m_numberOfRioters = m_rioters.size();
    for(unsigned int a=0; a<m_numberOfRioters; ++a)
    {
        Rioter* currentRioter = m_rioters[a];
        currentRioter->update(timeElapsed, currentTime);
    }

    m_numberOfSquads = m_squads.size();

    for(unsigned int a=0; a<m_squads.size(); ++a)
    {
        Squad* currentSquad = m_squads[a];
        currentSquad->update(timeElapsed, currentTime);
    }



    // check for win/lose states

    if(m_numberOfRiotersHome >= m_numberOfRiotersHomeToWin)
    {
        m_win = 1;
    }
    if(m_numberOfRiotersDead >= m_numberOfRiotersDeadToLose)
    {
        m_lose = 1;
    }
    if(m_availablePolice <= 0 && m_activePolice <= 0)
    {
        m_lose = 1;
    }
}

void GameWorld::loadMatricesToShader(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{

//  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
//  ngl::Material m(ngl::Colour(0.2f,0.2f,0.2f, 1.0), ngl::Colour(0.32f,0.31f,0.3f, 1.f), ngl::Colour(0.77391f,0.77391f,0.77391f, 1.0));
//  m.setSpecularExponent(20.f);
//  m.loadToShader("material");

  ngl::ShaderLib *shader=ngl::ShaderLib::instance();


  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;

  M=mouseGlobalTX;
  MV=  M*cam->getViewMatrix();
  MVP= M*cam->getVPMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);

}


void GameWorld::draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX)
{

  ngl::Material a(ngl::Colour(0.2f,0.2f,0.2f, 1.0), ngl::Colour(0.32f,0.31f,0.3f, 1.), ngl::Colour(0.77391f,0.77391f,0.77391f, 1.0));
  a.setSpecularExponent(20.f);
  a.loadToShader("material");
  loadMatricesToShader(cam, mouseGlobalTX);
  m_streetMesh->draw();

  ngl::Material b(ngl::Colour(0.2f,0.2f,0.2f, 1.0), ngl::Colour(0.45f,0.45f,0.45f, 1.), ngl::Colour(0.77391f,0.77391f,0.77391f, 1.0));
  b.setSpecularExponent(20.f);
  b.loadToShader("material");
  loadMatricesToShader(cam, mouseGlobalTX);

  m_buildingMesh->draw();

  for(unsigned int a=0; a<m_numberOfRioters; ++a)
  {
      Rioter* currentRioter = m_rioters[a];
      currentRioter->draw(cam, mouseGlobalTX);
  }

  for(unsigned int a=0; a<m_squads.size(); ++a)
  {
      Squad* currentSquad = m_squads[a];
      currentSquad->draw(cam, mouseGlobalTX);
  }


}

void GameWorld::createSquad(int size)
{
    if (m_availablePolice - size < 0)
    {
        std::cout<<"Not enough police available!"<<std::endl;
    }
    else
    {
        Squad* newSquad = new Squad(this, size, m_policeStation, 0.5f, m_policeMesh);
        m_squads.push_back(newSquad);

        m_activePolice += size;
        m_availablePolice -= size;
    }
}

void GameWorld::squadTarget(Squad* selectedSquad, ngl::Vec3 target)
{

//    std::vector<ngl::Vec3> path = m_cellGraph->findPath(m_entityMgr->getEntityFromID(selectedSquad->getID()), target);

    if(m_cellGraph->posIsInCell(target))
    {
        selectedSquad->setTarget(target);
    }
//      selectedSquad->findPath(target);
}

void GameWorld::registerLua(lua_State* _L)
{
  // Set up LUA state
  luaL_dofile(_L, "lua/GameWorld.lua");
  luaL_openlibs(_L);

    luabridge::getGlobalNamespace(_L)
        .beginClass<GameWorld>("GameWorld")
            .addProperty("m_initialNumberOfRioters", &GameWorld::getInitialNumberOfRioters, &GameWorld::setInitialNumberOfRioters)
            .addProperty("m_availablePolice", &GameWorld::getAvailablePolice, &GameWorld::setAvailablePolice)
            .addProperty("m_cellGraphFile", &GameWorld::getCellGraphFile, &GameWorld::setCellGraphFile)
            .addProperty("m_numberOfRiotersDeadToLose", &GameWorld::getNumberOfRiotersDeadToLose, &GameWorld::setNumberOfRiotersDeadToLose)
            .addProperty("m_numberOfRiotersHomeToWin", &GameWorld::getNumberOfRiotersHomeToWin, &GameWorld::setNumberOfRiotersHomeToWin)
            .addProperty("m_streetMeshFile", &GameWorld::getStreetMeshFile, &GameWorld::setStreetMeshFile)
            .addProperty("m_buildingMeshFile", &GameWorld::getBuildingMeshFile, &GameWorld::setBuildingMeshFile)
            .addFunction("setPoliceStation", &GameWorld::setPoliceStation)
        .endClass();

    luabridge::push(L, this);
    lua_setglobal(L, "gameWorld");
}
