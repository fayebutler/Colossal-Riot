-- set up initial police variables

makePolice = function()
   police.m_health = 100
   police.m_morale = 100
   police.m_rage = 1
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
  if police.m_health <= 0 then
    stateMachine:changeState("dead")
  end
end

global["exit"] = function()
end



-- work state

work = {}
work["enter"] = function()
  print("POLICE work enter")
end

work["execute"] = function()
  print("POLICE work execute")
  police.m_morale = police.m_morale - 1
  police:attack(1)
  if police.m_morale < 30 then
    stateMachine:changeState("flee")
  end
end

work["exit"] = function()
  print("POLICE work exit")
end



-- flee state

flee = {}
flee["enter"] = function()
  print("POLICE flee enter")
end

flee["execute"] = function()
  print("POLICE flee execute")
  police.m_morale = police.m_morale + 5
  if police.m_morale > 75 then
    stateMachine:changeState("work")
  end
end

flee["exit"] = function()
  print("POLICE flee exit")
end



-- dead state

dead = {}
dead["enter"] = function()
  print("POLICE dead enter")
end

dead["execute"] = function()
  print("PO PO DEAD")
end

dead["exit"] = function()
  print("POLICE dead exit")
end
