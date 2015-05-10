-- set up initial rioter variables

makeRioter = function()

   rioter.m_health = 80 + math.random(20)
   rioter.m_morale = 50 + math.random(50)
   rioter.m_rage = math.random(80)
   rioter.m_damage = 3.5

   stateMachine.m_currentState = "roam"
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
--    if rioter.m_health <= 0 then
--        stateMachine:changeState("dead")
--    end
  end

  if stateMachine.m_currentState ~= "home" then
--    if rioter.m_morale <= 0 then
--        stateMachine:changeState("home")
--    end
  end
end


global["exit"] = function()
end



-- protest state

protest = {}
protest["enter"] = function()

   rioter:wander(0.0)
   rioter:pursuit(1.0)
   rioter:evade(0.0)
   rioter:seek(0.0)
   rioter:arrive(0.0)

   rioter:cohesion(0.8)
   rioter:separation(0.8)
   rioter:alignment(0.5)

   rioter:protestCohesion(1.0)

end

protest["execute"] = function()

  rioter:cohesion(0.02*rioter.m_rage)

  rioter:checkValidTarget(3.0, 0.0)

  if rioter:getTargetID() >= 0 then
    rioter:wander(0.0)
    rioter:separation(0.2)
    if(rioter:targetWithinReach(2.0) == true) then
        rioter:attack()
    end
  else
    rioter:wander(0.5)
    rioter:separation(0.2)
  end

  if rioter.m_rage < 30 then
    stateMachine:changeState("roam")
  end
  if rioter.m_health < 30 then
    stateMachine:changeState("flee")
  end

  rioter.m_rage = rioter.m_rage + (rioter:getPoliceInfluence() * 0.001);

end

protest["exit"] = function()
end



-- roam state

roam = {}
roam["enter"] = function()

   rioter:wander(0.5)
   rioter:pursuit(0.0)
   rioter:evade(0.0)
   rioter:seek(0.0)
   rioter:arrive(0.0)

   rioter:cohesion(0.8)
   rioter:separation(0.8)
   rioter:alignment(0.5)

   rioter:protestCohesion(0.0)

end

roam["execute"] = function()

  rioter:cohesion(0.02*rioter.m_rage)


  if rioter.m_rage > 60 then
    stateMachine:changeState("protest")
  end
  if rioter.m_health < 30 then
--    stateMachine:changeState("flee")
  end

--  rioter.m_rage = rioter.m_rage + (rioter:getPoliceInfluence() * 0.0005);

end

roam["exit"] = function()
--  print("LUA RIOTER roam exit")
end



-- flee state

flee = {}
flee["enter"] = function()

   rioter:wander(0.0)
   rioter:pursuit(0.0)
   rioter:evade(1.0)
   rioter:seek(0.0)
   rioter:arrive(0.0)

   rioter:cohesion(0.2)
   rioter:separation(0.8)
   rioter:alignment(0.3)

   rioter:protestCohesion(0.0)

end

flee["execute"] = function()

  rioter:checkValidTarget(1.0, 50.0)

  if rioter:getTargetID() < 0 then
    stateMachine:changeState("protest")
  end

  if rioter.m_morale > 75 then
    stateMachine:changeState("protest")
  end

  rioter.m_morale = rioter.m_morale + 0.2
  rioter.m_rage = rioter.m_rage - 0.1;

end

flee["exit"] = function()
--  print("LUA RIOTER flee exit")
end



-- dead state

dead = {}
dead["enter"] = function()

   rioter:wander(0.0)
   rioter:pursuit(0.0)
   rioter:evade(0.0)
   rioter:seek(0.0)
   rioter:arrive(0.0)

   rioter:cohesion(0.0)
   rioter:separation(0.0)
   rioter:alignment(0.0)

   rioter:protestCohesion(0.0)

end

dead["execute"] = function()

  print("I'M DEAD")
  rioter.m_health = 0
end

--dead["exit"] = function()
--end



-- home state

home = {}
home["enter"] = function()

   rioter:findPathHome()

   rioter:wander(0.0)
   rioter:pursuit(0.0)
   rioter:evade(0.0)
   rioter:seek(1.0)
   rioter:arrive(0.0)

   rioter:cohesion(0.0)
   rioter:separation(0.0)
   rioter:alignment(0.0)

   rioter:protestCohesion(0.0)

end

home["execute"] = function()

  print("going home")
  rioter.m_morale = 0

end

home["exit"] = function()
end



-- limits

limits = {}
limits["check"] = function()

    if rioter.m_health > 100 then
        rioter.m_health = 100
    end
    if rioter.m_health < 0 then
        rioter.m_health = 0
    end

    if rioter.m_morale > 100 then
        rioter.m_morale = 100
    end
    if rioter.m_morale < 0 then
        rioter.m_morale = 0
    end

    if rioter.m_rage > 100 then
        rioter.m_rage = 100
    end
    if rioter.m_rage < 0 then
        rioter.m_rage = 0
    end

--    if rioter.m_damage > 1 then
--        rioter.m_damage = 1
--    end
--    if rioter.m_damage < 0 then
--        rioter.m_damage = 0
--    end

end
