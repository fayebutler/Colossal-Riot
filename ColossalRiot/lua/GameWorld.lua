-- percentage to win, percentage to lose

level1 = function()
  gameWorld.m_worldMeshFile = "drawMesh.obj"
  gameWorld.m_cellGraphFile = "navMesh.obj"
  gameWorld:setPoliceStation(2.0,0,-38)
  gameWorld.m_initialNumberOfRioters = 100
  gameWorld.m_availablePolice = 125
  gameWorld.m_numberOfTrees = 20
  gameWorld.m_numberOfStreetLights = 100
end

level2 = function()
  gameWorld.m_worldMeshFile = "drawMesh.obj"
  gameWorld.m_cellGraphFile = "navMesh.obj"
  gameWorld.m_initialNumberOfRioters = 200
  gameWorld.m_availablePolice = 100
  gameWorld.m_numberOfTrees = 20
  gameWorld.m_numberOfStreetLights = 100
end

level3 = function()
  gameWorld.m_worldMeshFile = "drawMesh.obj"
  gameWorld.m_cellGraphFile = "navMesh.obj"
  gameWorld.m_initialNumberOfRioters = 300
  gameWorld.m_availablePolice = 75
  gameWorld.m_numberOfTrees = 20
  gameWorld.m_numberOfStreetLights = 100
end

level4 = function()
  gameWorld.m_worldMeshFile = "drawMesh.obj"
  gameWorld.m_cellGraphFile = "navMesh.obj"
  gameWorld.m_initialNumberOfRioters = 400
  gameWorld.m_availablePolice = 50
  gameWorld.m_numberOfTrees = 20
  gameWorld.m_numberOfStreetLights = 100
end

level5 = function()
  gameWorld.m_worldMeshFile = "drawMesh.obj"
  gameWorld.m_cellGraphFile = "navMesh.obj"
  gameWorld.m_initialNumberOfRioters = 500
  gameWorld.m_availablePolice = 25
  gameWorld.m_numberOfTrees = 20
  gameWorld.m_numberOfStreetLights = 100
end
