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


-- work state

work = {}
work["enter"] = function(currentState, previousState, morale)
  print("POLICE work enter")
  morale = morale
  return currentState, previousState, morale
end

work["execute"] = function(currentState, previousState, morale)
  print("POLICE work execute")
  morale = morale - 10
  if morale < 15 then
    previousState = currentState
    currentState = "flee"
    work.exit(currentState, previousState, morale)
    flee.enter(currentState, previousState, morale)
  end
  return currentState, previousState, morale
end

work["exit"] = function(currentState, previousState, morale)
  print("POLICE work exit")
  morale = morale
  return currentState, previousState, morale
end



-- flee state

flee = {}
flee["enter"] = function(currentState, previousState, morale)
  print("POLICE flee enter")
  morale = morale
  return currentState, previousState, morale
end

flee["execute"] = function(currentState, previousState, morale)
  print("POLICE flee execute")
  morale = morale + 10
  if morale > 50 then
    previousState = currentState
    currentState = "work"
    flee.exit(currentState, previousState, morale)
    work.enter(currentState, previousState, morale)
  end
  return currentState, previousState, morale
end

flee["exit"] = function(currentState, previousState, morale)
  print("POLICE flee exit")
  morale = morale
  return currentState, previousState, morale
end

