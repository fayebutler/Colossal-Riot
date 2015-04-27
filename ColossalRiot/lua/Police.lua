-- set up initial police variables

makePolice = function()
   police.m_health = 150
   police.m_morale = 100
   police.m_rage = 20
   police.m_damage = 0.01
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
end

global["exit"] = function()
end



-- work state

work = {}
work["enter"] = function()

  print("LUA POLICE work enter")
--  police:pursuit(1.0)
--  police:setTargetID(0)
end

work["execute"] = function()
  print("LUA POLICE work execute")
  police.m_morale = police.m_morale - 0.2
  police:attack()

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
