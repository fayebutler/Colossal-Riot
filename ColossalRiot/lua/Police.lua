-- set up initial police variables

makePolice = function()
   police.m_health = 150
   police.m_morale = 100
   police.m_rage = 20
   police.m_damage = 0.1
   --police:wander(0.2)
   stateMachine.m_currentState = "work"
   stateMachine.m_globalState = "global"
end



-- TEMPLATE STATE

STATE = {}
STATE["enter"] = function()
end

STATE["execute"] = function()
  if CONDITION then
    stateMachine:changeState(NEWSTATE)
  end
end

STATE["exit"] = function()
end



-- global state

global = {}
global["enter"] = function()
end

global["execute"] = function()
  if stateMachine.m_currentState ~= "dead" then
    if police.m_health <= 0 then
        stateMachine:changeState("dead")
    end
  end
  if stateMachine.m_currentState ~= "move" then
    if police.m_isMoving == true then
        stateMachine:changeState("move")
    end
  end
end

global["exit"] = function()
end

-- move state

move = {}
move["enter"] = function()
  police:pursuit(0.0)
  police:wander(0.0)
  police:squadCohesion(0.0)
  police:cohesion(0.1)
  police:separation(0.1)
  police:alignment(0.0)
  police:seek(1.0)
  print("LUA POLICE move enter")
end

move["execute"] = function()
  print("LUA POLICE move execute")
  if police.m_isMoving == false then
    stateMachine:changeState(stateMachine.m_previousState)
  end
end

move["exit"] = function()
  print("LUA POLICE move exit")
end

-- work state

work = {}
work["enter"] = function()

  print("LUA POLICE work enter")
 -- police:checkValidTarget(1.0, 20.0)
 -- police:pursuit(1.0)
   police:cohesion(0.4)
   police:separation(0.8)
   police:alignment(0.3)
   police:seek(0.0)
end

work["execute"] = function()
  print("LUA POLICE work execute")
  print(police.m_targetID)
  police:checkValidTarget(3.0, 20.0)
--  if police.m_targetID >= 0 then
--    police:wander(0.0)
--    police:attack()
--  else
  police:wander(0.5)
  police:squadCohesion(0.4)
--  end

  if police.m_morale < 30 then
    stateMachine:changeState("flee")
  end
end

work["exit"] = function()
--  print("LUA POLICE work exit")
end



-- flee state

flee = {}
flee["enter"] = function()
    police:pursuit(0.0)
--  police:setTargetID(1)
  print("LUA POLICE flee enter")
end

flee["execute"] = function()
  print("LUA POLICE flee execute")
  police.m_morale = police.m_morale + 0.5
  if police.m_morale > 75 then
    stateMachine:changeState("work")
  end
end

flee["exit"] = function()
--  print("LUA POLICE flee exit")
end



-- dead state

dead = {}
dead["enter"] = function()
--  police:pursuit(0.0)
--  police:wander(1.0)
  print("LUA POLICE dead enter")
end

dead["execute"] = function()
  print("LUA POLICE dead execute")
  police.m_health = 0
end

--dead["exit"] = function()
--  print("LUA POLICE dead exit")
--end
