-- TEMPLATE STATE

STATE = {}
STATE["enter"] = function(currentState, previousState, morale)
  return currentState, previousState, morale
end

STATE["execute"] = function(currentState, previousState, morale)
  if CONDITION then
    previousState = currentState
    currentState = "NEXTSTATE"
    STATE.exit(currentState, previousState, morale)
    NEXTSTATE.enter(currentState, previousState, morale)
  end
  return currentState, previousState, morale
end

STATE["exit"] = function(currentState, previousState, morale)
  return currentState, previousState, morale
end


-- protest state

protest = {}
protest["enter"] = function(currentState, previousState, morale)
  print("protest enter")
  morale = morale
  return currentState, previousState, morale
end

protest["execute"] = function(currentState, previousState, morale)
  print("protest execute")
  morale = morale - 15
  if morale < 30 then
    previousState = currentState
    currentState = "flee"
    protest.exit(currentState, previousState, morale)
    flee.enter(currentState, previousState, morale)
  end
  return currentState, previousState, morale
end

protest["exit"] = function(currentState, previousState, morale)
  print("protest exit")
  morale = morale
  return currentState, previousState, morale
end



-- flee state

flee = {}
flee["enter"] = function(currentState, previousState, morale)
  print("flee enter")
  morale = morale
  return currentState, previousState, morale
end

flee["execute"] = function(currentState, previousState, morale)
  print("flee execute")
  morale = morale + 5
  if morale > 75 then
    previousState = currentState
    currentState = "protest"
    flee.exit(currentState, previousState, morale)
    protest.enter(currentState, previousState, morale)
  end
  return currentState, previousState, morale
end

flee["exit"] = function(currentState, previousState, morale)
  print("flee exit")
  morale = morale
  return currentState, previousState, morale
end

