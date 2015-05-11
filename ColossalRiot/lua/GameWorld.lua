------------------------------------------------------------------------------------------------------------------------
-- file: GameWorld.lua "lua/GameWorld.lua"
-- brief: a Lua file used to edit world settings within the game corresponding to the level buttons on the menu
-- author: Will Herbert
-- version: 1.0
-- last revision: 10/05/2015 updated to add documentation and instructions
------------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------
-- properties exposed from C++
-- m_cellGraphFile              = the .obj file used to generate the cell graph for path planning and cell space partitioning
-- m_streetMeshFile             = the .obj file used to draw the street and surrounding non-playable geometry
-- m_buildingMeshFile           = the .obj file used to draw the buildings in the playable area
-- m_initialNumberOfRioters     = how many rioters are spawned at the start of the game
-- m_availablePolice            = how many police are available to spawn in squads
-- m_numberOfRiotersHomeToWin   = the number of rioters that are required to go home for the player to win
-- m_numberOfRiotersDeadToLose  = the number of rioters that are required to die for the player to lose
-- m_numberOfTrees              = how many trees are spawned at the start of the game
-- m_numberOfStreetLights       = how many street lights are spawned at the start of the game
--
-- functions exposed from C++
-- setPoliceStation(float, float, float) = sets where the police squads spawn in the level
------------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------
-- level 1
------------------------------------------------------------------------------------------------------------------------
level1 = function()
  gameWorld.m_cellGraphFile = "models/navMesh.obj"
  gameWorld.m_streetMeshFile = "models/streetMesh1.obj"
  gameWorld.m_buildingMeshFile = "models/buildingsMesh1.obj"

  gameWorld:setPoliceStation(0,0,0)
  gameWorld.m_initialNumberOfRioters = 100
  gameWorld.m_availablePolice = 125
  gameWorld.m_numberOfRiotersHomeToWin = 25
  gameWorld.m_numberOfRiotersDeadToLose = 75

  gameWorld.m_numberOfTrees = 20
  gameWorld.m_numberOfStreetLights = 100

  gameWorld:addProtestPosition(0.0,0.0,0.0)
end

------------------------------------------------------------------------------------------------------------------------
-- level 2
------------------------------------------------------------------------------------------------------------------------
level2 = function()
  gameWorld.m_cellGraphFile = "models/navMesh2.obj"
  gameWorld.m_streetMeshFile = "models/streetMesh1.obj"
  gameWorld.m_buildingMeshFile = "models/buildingsMesh2.obj"

  gameWorld:setPoliceStation(34.0,0, 46)
  gameWorld.m_initialNumberOfRioters = 400
  gameWorld.m_availablePolice = 100
  gameWorld.m_numberOfRiotersHomeToWin = 100
  gameWorld.m_numberOfRiotersDeadToLose = 70

  gameWorld.m_numberOfTrees = 20
  gameWorld.m_numberOfStreetLights = 100

  gameWorld:addProtestPosition(14.0,0.0,-10.0)
  gameWorld:addProtestPosition(-10.0,0.0,-50.0)
end

------------------------------------------------------------------------------------------------------------------------
-- level 3
------------------------------------------------------------------------------------------------------------------------
level3 = function()
  gameWorld.m_cellGraphFile = "models/navMesh2.obj"
  gameWorld.m_streetMeshFile = "models/streetMesh1.obj"
  gameWorld.m_buildingMeshFile = "models/buildingsMesh2.obj"

  gameWorld:setPoliceStation(0,0,0)
  gameWorld.m_initialNumberOfRioters = 300
  gameWorld.m_availablePolice = 75
  gameWorld.m_numberOfRiotersHomeToWin = 175
  gameWorld.m_numberOfRiotersDeadToLose = 50

  gameWorld.m_numberOfTrees = 20
  gameWorld.m_numberOfStreetLights = 100

  gameWorld:addProtestPosition(0.0,0.0,0.0)
end

------------------------------------------------------------------------------------------------------------------------
-- level 4
------------------------------------------------------------------------------------------------------------------------
level4 = function()
  gameWorld.m_cellGraphFile = "models/navMesh2.obj"
  gameWorld.m_streetMeshFile = "models/streetMesh1.obj"
  gameWorld.m_buildingMeshFile = "models/buildingsMesh2.obj"

  gameWorld:setPoliceStation(2.0,0,-38)
  gameWorld.m_initialNumberOfRioters = 400
  gameWorld.m_availablePolice = 50
  gameWorld.m_numberOfRiotersHomeToWin = 325
  gameWorld.m_numberOfRiotersDeadToLose = 40

  gameWorld.m_numberOfTrees = 20
  gameWorld.m_numberOfStreetLights = 100

  gameWorld:addProtestPosition(0.0,0.0,0.0)
end

------------------------------------------------------------------------------------------------------------------------
-- level 5
------------------------------------------------------------------------------------------------------------------------
level5 = function()
  gameWorld.m_cellGraphFile = "models/navMesh.obj"
  gameWorld.m_streetMeshFile = "models/streetMesh1.obj"
  gameWorld.m_buildingMeshFile = "models/buildingsMesh1.obj"

  gameWorld:setPoliceStation(0,0,0)
  gameWorld.m_initialNumberOfRioters = 500
  gameWorld.m_availablePolice = 25
  gameWorld.m_numberOfRiotersHomeToWin = 475
  gameWorld.m_numberOfRiotersDeadToLose = 30

  gameWorld.m_numberOfTrees = 20
  gameWorld.m_numberOfStreetLights = 100

  gameWorld:addProtestPosition(0.0,0.0,0.0)
end
