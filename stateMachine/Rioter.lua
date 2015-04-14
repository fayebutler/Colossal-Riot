-- set up initial rioter variables

makeRioter = function()
   rioter.m_health = 100
   rioter.m_morale = 100
   rioter.m_rage = 17
   stateMachine.m_currentState = "protest"
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
  if rioter.m_health <= 0 then
    stateMachine:changeState("dead")
  end
end

global["exit"] = function()
end



-- protest state

protest = {}
protest["enter"] = function()
  print("RIOTER protest enter")
end

protest["execute"] = function()
  print("RIOTER protest execute")
  rioter.m_morale = rioter.m_morale - 5
  rioter:attack(1)
  if rioter.m_morale < 30 then
    stateMachine:changeState("flee")
  end
end

protest["exit"] = function()
  print("RIOTER protest exit")
end



-- flee state

flee = {}
flee["enter"] = function()
  print("RIOTER flee enter")
end

flee["execute"] = function()
  print("RIOTER flee execute")
  rioter.m_morale = rioter.m_morale + 5
  if rioter.m_morale > 75 then
    stateMachine:changeState("protest")
  end
end

flee["exit"] = function()
  print("RIOTER flee exit")
end



-- dead state

dead = {}
dead["enter"] = function()
  print("RIOTER dead enter")
end

dead["execute"] = function()
  print("RIOTER DEAD")
end

dead["exit"] = function()
  print("RIOTER dead exit")
end
