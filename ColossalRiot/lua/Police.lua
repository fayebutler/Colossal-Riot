-- set up initial police variables

makePolice = function()

   police.m_health = 150
   police.m_morale = 100
   police.m_rage = 20
   police.m_damage = 10.0

   stateMachine.m_currentState = "patrol"
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


--  if stateMachine.m_currentState ~= "dead" then
--    if police.m_health <= 0 then
--        stateMachine:changeState("dead")
--    end
--  end

  if stateMachine.m_currentState ~= "home" then
    if police.m_morale <= 0 then
        stateMachine:changeState("home")
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

   police:wander(0.3)
   police:pursuit(0.0)
   police:evade(0.0)
   police:seek(10.0)
   police:arrive(0.0)

   police:cohesion(0.0)
   police:separation(0.4)
   police:alignment(0.0)

   police:squadCohesion(0.0)

end

move["execute"] = function()
--  print("LUA POLICE move execute")
  if police.m_isMoving == false then
    stateMachine:changeState(stateMachine.m_previousState)
  end
end

move["exit"] = function()
end



-- patrol state

patrol = {}
patrol["enter"] = function()


   police:wander(0.5)
   police:pursuit(0.0)
   police:evade(0.0)
   police:seek(0.0)
   police:arrive(0.0)

   police:cohesion(0.2)
   police:separation(0.5)
   police:alignment(0.0)

   police:squadCohesion(0.6)

   police.m_rage = 5.0

end

patrol["execute"] = function()

--  print("LUA POLICE patrol execute")
  police:squadCohesion(0.6)

  if police.m_morale < 20 then
    stateMachine:changeState("flee")
  end
  if police.m_health < 20 then
    stateMachine:changeState("flee")
  end

end

patrol["exit"] = function()
end



-- defensive state

defensive = {}
defensive["enter"] = function()

   police:wander(0.0)
   police:pursuit(1.0)
   police:evade(0.0)
   police:seek(0.0)
   police:arrive(0.0)

   police:cohesion(0.0)
   police:separation(0.5)
   police:alignment(0.0)

   police:squadCohesion(0.6)

   police.m_rage = 20.0

end

defensive["execute"] = function()

--  print("LUA POLICE defensive execute")

  police:checkValidTarget(1.0, 20.0)
  police:checkValidPursuitRange(32.0)

  if police:getTargetID() >= 0 then
    police:wander(0.0)
    police:squadCohesion(0.0)
    if(police:targetWithinReach(2.0) == true) then
        police:attack()
    end
  else
    police:wander(0.5)
    police:squadCohesion(0.2)
  end

  if police.m_morale < 20 then
    stateMachine:changeState("flee")
  end
end

defensive["exit"] = function()
end



-- aggressive state

aggressive = {}
aggressive["enter"] = function()

   police:wander(0.0)
   police:pursuit(1.0)
   police:evade(0.0)
   police:seek(0.0)
   police:arrive(0.0)

   police:cohesion(0.0)
   police:separation(0.5)
   police:alignment(0.0)

   police:squadCohesion(0.5)

   police.m_rage = 70.0

end

aggressive["execute"] = function()

--  print("LUA POLICE aggressive execute")

  police:checkValidTarget(3.0, 0.0)
  police:checkValidPursuitRange(64.0)

  if police:getTargetID() >= 0 then
    police:wander(0.0)
    police:squadCohesion(0.0)
    police:separation(0.0)
    if(police:targetWithinReach(2.0) == true) then
 --       print("attacking")
        police:attack()
    end
  else
--    print("no target")
    police:wander(0.5)
    police:squadCohesion(0.5)
  end


  if police.m_morale < 20 then
    stateMachine:changeState("flee")
  end
end

aggressive["exit"] = function()
end



-- wall state

wall = {}
wall["enter"] = function()

   police:wander(0.0)
   police:pursuit(0.0)
   police:evade(0.0)
   police:seek(0.0)
   police:arrive(1.0)

   police:cohesion(0.1)
   police:separation(0.1)
   police:alignment(0.3)

   police:squadCohesion(0.0)

   police.m_rage = 40.0

end

wall["execute"] = function()

--  print("LUA POLICE wall execute")
  if police.m_morale < 20 then
    stateMachine:changeState("flee")
  end
end

wall["exit"] = function()
end



-- flee state

flee = {}
flee["enter"] = function()

   police:wander(0.0)
   police:pursuit(0.0)
   police:evade(1.0)
   police:seek(0.0)
   police:arrive(0.0)

   police:cohesion(0.4)
   police:separation(0.8)
   police:alignment(0.3)

   police:squadCohesion(0.4)

end

flee["execute"] = function()
  police.m_morale = police.m_morale + 0.5
  if police.m_morale > 75 then
    stateMachine:changeState("patrol")
  end
end

flee["exit"] = function()
--  print("LUA POLICE flee exit")
end



-- dead state

dead = {}
dead["enter"] = function()

   police:wander(0.0)
   police:pursuit(0.0)
   police:evade(0.0)
   police:seek(0.0)
   police:arrive(0.0)

   police:cohesion(0.0)
   police:separation(0.0)
   police:alignment(0.0)

   police:squadCohesion(0.0)

end

dead["execute"] = function()
--  print("LUA POLICE dead execute")
  police.m_health = 0
end

--dead["exit"] = function()
--  print("LUA POLICE dead exit")
--end



-- home state

home = {}
home["enter"] = function()

   police:findPathHome()

   police:wander(0.0)
   police:pursuit(0.0)
   police:evade(0.0)
   police:seek(1.0)
   police:arrive(0.0)

   police:cohesion(0.0)
   police:separation(0.0)
   police:alignment(0.0)

   police:squadCohesion(0.0)

end

home["execute"] = function()
 -- print("LUA RIOTER home execute")
  --rioter.m_morale = 0
end

home["exit"] = function()
  print("LUA RIOTER home exit")
end



-- limits

limits = {}
limits["check"] = function()

    if police.m_health > 200 then
        police.m_health = 200
    end
    if police.m_health < 0 then
        police.m_health = 0
    end

    if police.m_morale > 100 then
        police.m_morale = 100
    end
    if police.m_morale < 0 then
        police.m_morale = 0
    end

    if police.m_rage > 100 then
        police.m_rage = 100
    end
    if police.m_rage < 0 then
        police.m_rage = 0
    end

--    if police.m_damage > 1 then
--        police.m_damage = 1
--    end
--    if police.m_damage < 0 then
--        police.m_damage = 0
--    end

end
