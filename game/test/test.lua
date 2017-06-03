test = 1
state = "startup"
quit = false
screenShotFilename = "out/test-screenshot.png"
screenShotWait = 100
menuWait = 100
createTestImages = 0

function Setup()
   uae_write_symbol16("_script_port", 0)
   log = io.open("lua.log", "w")
   io.output(log)
   --uae_warp()
   io.write("\n\n=== test started ===\n")
end


function Quit()
   quit = true
   io.flush()
   log:close()
   uae_quit()
end


function Reset()
   uae_reset()
end


function Write(symbol, value)
   io.write("ram write: ", symbol, " = ", value, "\n")
   uae_write_symbol16(symbol, value)
end


function Write32(symbol, value)
   io.write("ram write32: ", symbol, " = ", value, "\n")
   uae_write_symbol32(symbol, value)
end


function CheckScreenshot(filename)
   local screenshot1 = io.open(screenShotFilename, "rb")
   local screenshot2
   local screenshot1Data = screenshot1:read("*all")

   screenshot1:close()
   if createTestImages == 1 then
      io.write("CREATED: ", screenShotFilename, " ==> ", filename, "\n")      
      screenshot2 = io.open(filename, "wb")
      screenshot2:write(screenshot1Data)
      screenshot2:close()
   else
      screenshot2 = io.open(filename, "rb")      
      local screenshot2Data = screenshot2:read("*all")
      screenshot2:close()      
      if screenshot1Data ~= screenshot2Data then
	 io.write("FAIL: ", screenShotFilename, " != ", filename, "\n")
	 io.flush()
	 quit = true
      else
	 io.write("PASS: ", screenShotFilename, " == ", filename, "\n")
      end
   end

end


function GameScreenshot(_state)
   
   if screenShotState == 0 or screenShotState == nil then
      screenShotState = 1
      Write("_script_port",  _state.screenShotFrame + 0x8000)
   elseif screenShotState == 1 then
      if uae_peek_symbol32("_game_paused") == 1 then
	 screenShotState = 2
	 screenShotFrame = uae_peek_symbol32("_hw_verticalBlankCount")
      end
   elseif screenShotState == 2 then
      if uae_peek_symbol32("_hw_verticalBlankCount") > screenShotFrame+screenShotWait then
	 screenShotState = 3
	 screenShotFrame = uae_peek_symbol32("_hw_verticalBlankCount")
	 uae_screenshot(screenShotFilename)
      end
   elseif screenShotState == 3 then
      if uae_peek_symbol32("_hw_verticalBlankCount") > screenShotFrame+screenShotWait then
	 CheckScreenshot(_state.filename)
	 Write("_script_port", string.byte(' '))
	 --Write32("_game_paused", 0)	 
	 screenShotState = 4
      end
   elseif screenShotState == 4 then
      if uae_peek_symbol32("_game_paused") == 0 then
	 screenShotState = 0
	 return true
      else
	 Write("_script_port", string.byte(' '))
	 --Write32("_game_paused", 0)
      end
   end
   return false
end


function Screenshot(_state)
   if screenShotState == 0 or screenShotState == nil then
      screenShotFrame = uae_peek_symbol32("_hw_verticalBlankCount")
      screenShotState = 1
   elseif screenShotState == 1 then
      if uae_peek_symbol32("_hw_verticalBlankCount") > screenShotFrame+screenShotWait or
         uae_peek_symbol32("_hw_verticalBlankCount") < screenShotFrame then	 
	 screenShotState = 2
	 screenShotFrame = uae_peek_symbol32("_hw_verticalBlankCount")
	 uae_screenshot(screenShotFilename)
      end
   elseif screenShotState == 2 then
      if uae_peek_symbol32("_hw_verticalBlankCount") > screenShotFrame+screenShotWait or
         uae_peek_symbol32("_hw_verticalBlankCount") < screenShotFrame then
	 CheckScreenshot(_state.filename)
	 screenShotFrame = uae_peek_symbol32("_hw_verticalBlankCount")
	 screenShotState = 3
      end
   elseif screenShotState == 3 then
      if uae_peek_symbol32("_hw_verticalBlankCount") > screenShotFrame+screenShotWait or
      uae_peek_symbol32("_hw_verticalBlankCount") < screenShotFrame then      
	 screenShotState = 0
	 return true
      end
   end
   return false
end

function WaitForMessage(_state)
   message = uae_peek_string("_message_message")
   if message == state then
      io.write("_message_message ", state, " == ", message, "\n")         
      return true
   end
   io.write("_message_message ", state, " ~= ", message, "\n")      
   return false
end

setup = {
   ["startup"] = {
      exitState = Setup
   }
}

mainMenu = {
   ["booting"] = {
      waitFrames = 250,
      next = "disable scroller",      
   },
   ["disable scroller"] = {
      writeEntry = {"_script_port",  string.byte(' ')},
      waitFrames = 500,
      next = "main menu hiscore screenshot",
   },
   ["main menu hiscore screenshot"] = {
      filename = "test/screenshots/mainmenuhiscore.png",
      transition = Screenshot,
      next = "enter",
   },
   ["enter"] = {
      writeEntry = {"_script_port", 10},
      waitFrames = 250,
      next = "main menu screenshot",
   },
   ["main menu screenshot"] = {
      filename = "test/screenshots/mainmenu.png",
      transition = Screenshot,
      next = "down",
   },
   ["down"] = {
      writeEntry = {"_script_port",  5}, -- joystick down,
      waitFrames = 100,
      next = "enter2"
   },
   ["enter2"] = {
      writeEntry = {"_script_port",  10}, -- enter
      waitFrames = 100,
      next = "toggled menu item screenshot"
   },
   ["toggled menu item screenshot"] = {
      filename = "test/screenshots/toggledmenu.png",
      transition = Screenshot,
      writeExit = {"_script_port", 1}, -- joystick up
      next = "pause"
   },
   ["pause"] = {
      waitFrames = 100
   }
}

mainMenu2 = {
   ["booting"] = {
      waitFrames = 250,
      next = "disable scroller",      
   },
   ["disable scroller"] = {
      writeEntry = {"_script_port",  string.byte(' ')},
      waitFrames = 500,
      next = "main menu hiscore screenshot",
   },
   ["main menu hiscore screenshot"] = {
      filename = "test/screenshots/mainmenuhiscore-2.png",
      transition = Screenshot,
      next = "enter",
   },
   ["enter"] = {
      writeEntry = {"_script_port", 10},
      waitFrames = 250,
      next = "main menu screenshot",
   },
   ["main menu screenshot"] = {
      filename = "test/screenshots/mainmenu-2.png",
      transition = Screenshot,
   },   
}


level = {
   ["booting"] = {
      wait = {"_hw_verticalBlankCount", 1, 32},      
      next = "game start"
   },
   ["game start"] = {
      wait = {"_hw_verticalBlankCount", 0, 32},      
      next = "screenshot1",
   },      
   ["screenshot1"] = {
      screenShotFrame = 500,
      filename = "test/screenshots/screenshot1.png",
      transition = GameScreenshot,
      next = "screenshot2"
   },
   ["screenshot2"] = {
      screenShotFrame = 1000,
      filename = "test/screenshots/screenshot2.png",
      transition = GameScreenshot,
      next = "screenshot3"
   },   
   ["screenshot3"] = {
      screenShotFrame = 3001,
      filename = "test/screenshots/screenshot3.png",
      transition = GameScreenshot,
      next = "screenshot4"
   },
   ["screenshot4"] = {
      screenShotFrame = 3400,
      filename = "test/screenshots/screenshot4.png",
      transition = GameScreenshot,
      next = "waiting for level end"
   },         
   ["waiting for level end"] = {
      wait = {"_game_collectTotal", 0},
      next = "verify level parameters",
   },
   ["verify level parameters"] = {
      less = {{"_game_total", 965648, 32}},
      equal = {{"_game_player1Score", 9000, 32}, {"_game_player2Score", 13000, 32}}
   },
}


level1_2 = {
   ["booting"] = {
      wait = {"_hw_verticalBlankCount", 1, 32},      
      next = "game start"
   },
   ["game start"] = {
      wait = {"_hw_verticalBlankCount", 0, 32},      
      next = "screenshot1",
   },      
   ["screenshot1"] = {
      screenShotFrame = 500,
      filename = "test/screenshots/screenshot1_2.png",
      transition = GameScreenshot,
      next = "screenshot2"
   },
   ["screenshot2"] = {
      screenShotFrame = 1000,
      filename = "test/screenshots/screenshot2_2.png",
      transition = GameScreenshot,
      next = "screenshot3"
   },   
   ["screenshot3"] = {
      screenShotFrame = 3001,
      filename = "test/screenshots/screenshot3_2.png",
      transition = GameScreenshot,
      next = "screenshot4"
   },
   ["screenshot4"] = {
      screenShotFrame = 3400,
      filename = "test/screenshots/screenshot4_2.png",
      transition = GameScreenshot,
      next = "waiting for level end"
   },         
   ["waiting for level end"] = {
      wait = {"_game_collectTotal", 0},
      next = "verify level parameters",
   },
   ["verify level parameters"] = {
      less = {{"_game_total", 965648, 32}},
      equal = {{"_game_player1Score", 9000, 32}, {"_game_player2Score", 13000, 32}}
   },
}


newHiscore = {
   ["booting"] = {
      waitFrames = 2000,
      next = "ok"
   },
   ["ok"] = {
      writeEntry = {"_script_port", 10},
      waitFrames = 250,
      next = "a",
   },
   ["a"] = {
      waitFrames = 250,
      writeExit = {"_script_port", string.byte('a')},      
      next = "l"
   },
   ["l"] = {
      waitFrames = 250,
      writeExit = {"_script_port", string.byte('l')},
      next = "x"
   },
   ["x"] = {
      waitFrames = 250,
      writeExit = {"_script_port", string.byte('x')},
      next = "screenshot"
   },
   ["screenshot"] = {
      filename = "test/screenshots/enterhiscore.png",
      transition = Screenshot,
      next = "enter"
   },
   ["enter"] = {
      writeEntry = {"_script_port", 10},
      waitFrames = 250,
      next = "w",
   },
    ["w"] = {
      waitFrames = 250,
      writeExit = {"_script_port", string.byte('w')},
      next = "i"
   },
   ["i"] = {
      waitFrames = 250,
      writeExit = {"_script_port", string.byte('i')},
      next = "n"
   },
   ["n"] = {
      waitFrames = 250,
      writeExit = {"_script_port", string.byte('n')},
      next = "screenshot2"
   },
   ["screenshot2"] = {
      filename = "test/screenshots/enterhiscore2.png",
      transition = Screenshot,
      next = "enter2"
   },
   ["enter2"] = {
      writeEntry = {"_script_port", 10},
      waitFrames = 250,
   },   
}


newHiscore2 = {
   ["booting"] = {
      waitFrames = 2000,
      next = "ok"
   },
   ["ok"] = {
      writeEntry = {"_script_port", 10},
      waitFrames = 250,
      next = "a",
   },
   ["a"] = {
      waitFrames = 250,
      writeExit = {"_script_port", string.byte('1')},      
      next = "l"
   },
   ["l"] = {
      waitFrames = 250,
      writeExit = {"_script_port", string.byte('1')},
      next = "x"
   },
   ["x"] = {
      waitFrames = 250,
      writeExit = {"_script_port", string.byte('1')},
      next = "screenshot"
   },
   ["screenshot"] = {
      filename = "test/screenshots/enterhiscore-2.png",
      transition = Screenshot,
      next = "enter"
   },
   ["enter"] = {
      writeEntry = {"_script_port", 10},
      waitFrames = 250,
      next = "w",
   },
    ["w"] = {
      waitFrames = 250,
      writeExit = {"_script_port", string.byte('2')},
      next = "i"
   },
   ["i"] = {
      waitFrames = 250,
      writeExit = {"_script_port", string.byte('2')},
      next = "n"
   },
   ["n"] = {
      waitFrames = 250,
      writeExit = {"_script_port", string.byte('2')},
      next = "screenshot2"
   },
   ["screenshot2"] = {
      filename = "test/screenshots/enterhiscore2-2.png",
      transition = Screenshot,
      next = "enter2"
   },
   ["enter2"] = {
      writeEntry = {"_script_port", 10},
      waitFrames = 250,
   },   
}


reset = {
   ["booting"] = {
      enterState = Reset,
      wait = {"_menu_mode", 0, 32},
   }
}

restartReplay = {
   ["booting"] = {
      writeEntry = {"_script_port", 10},      
      waitFrames = 250,
      next = "select player screenshot"
   },
   ["select player screenshot"] = {
      filename = "test/screenshots/select-player.png",
      transition = Screenshot,
      next = "joystick right"
   },
   ["joystick right"] = {
      writeEntry = {"_script_port", 3},
      waitFrames = 10,
      next = "select player screenshot2"      
   },
   ["select player screenshot2"] = {
      filename = "test/screenshots/select-player2.png",
      transition = Screenshot,
      next = "joystick left"
   },
   ["joystick left"] = {
      writeEntry = {"_script_port", 7},
      waitFrames = 10,
      next = "LEVEL 1 READY!"
   },   
   ["LEVEL 1 READY!"] = {
      writeEntry = {"_script_port", 10},
      transition = WaitForMessage,
      next = "ready screenshot"
   },
   ["ready screenshot"] = {
      filename = "test/screenshots/ready.png",
      transition = Screenshot,
      writeExit = {"_script_port", string.byte('L')},
   }
}

tests = {
   { setup, "setup" },
   { level, "level 1, first pass"},
   { newHiscore, "new hiscore"},
   { mainMenu, "main menu"},
   { restartReplay, "restart replay"},
   { level1_2, "level 1, restart"},   
   { reset, "reset"},
   { level, "level 1,  second pass"},
   { newHiscore2, "new hiscore, second pass"},
   { mainMenu2, "main menu, second pass"},   
}


function EnterState(stateMachine, state)
   stateMachine[state].startFrame = uae_peek_symbol32("_hw_verticalBlankCount") 
   io.write("enter state ", state, " - frame = ", stateMachine[state].startFrame, "\n")
   if stateMachine[state].enterState then
      stateMachine[state].enterState(stateMachine[state])
   end
   if  stateMachine[state].writeEntry then
      Write(stateMachine[state].writeEntry[1], stateMachine[state].writeEntry[2])
   end
end


function Tick(stateMachine)
   if not quit then 
      local transition = false
      local asserts = false

      if stateMachine[state].equal then
	 asserts = true
	 for i, equal in ipairs(stateMachine[state].equal) do
	    if equal[3] == 32 then
	       local value = uae_peek_symbol32(equal[1])
	       if  value == equal[2] then
		  io.write("PASS: ", equal[1], " ", value, " == ", equal[2], "\n")
	       else
		  io.write("FAIL: ", equal[1], " ", value, " != ", equal[2], "\n")
		  quit = true
	       end
	    else
	       local value = uae_peek_symbol16(equal[1])
	       if (value == equal[2]) then
		  io.write("PASS: ", equal[1], " ", value, " == ", equal[2], "\n")
	       else
		  io.write("FAIL: ", equal[1], " ", value, " != ", equal[2], "\n")
		  quit = true
	       end
	    end
	 end
      end
      
      if not quit and stateMachine[state].less then
	 asserts = true
	 for i, less in ipairs(stateMachine[state].less) do
	    if less[3] == 32 then
	       local val32 = uae_peek_symbol32(less[1])
	       if val32 < less[2] then
		  io.write("PASS: ", less[1], " (", val32, ") < ", less[2], "\n")
	       else
		  io.write("FAIL: ", less[1], " (", val32, ") >= ", less[2], "\n")
		  quit = true
	       end
	    elseif (uae_peek_symbol16(less[1]) < less[2]) then
	    else
	       io.write("FAIL: ", less[1], " (",uae_peek_symbol16(less[1]), ") >= ", less[2], "\n")
	       quit = true
	    end
	 end
      end
      
      if not quit and asserts == false then
	 if stateMachine[state].wait then
	    if stateMachine[state].wait[3] == 32 then
	       if (uae_peek_symbol32(stateMachine[state].wait[1]) == stateMachine[state].wait[2]) then
		  io.write("trigger: ", stateMachine[state].wait[1], " == ", stateMachine[state].wait[2], "\n")
		  transition = true
	       end
	    else
	       local val16 = uae_peek_symbol16(stateMachine[state].wait[1])
	       --io.write("read16: ", stateMachine[state].wait[1], " == ", val16, "\n")
	       if (val16 == stateMachine[state].wait[2]) then
		  io.write("trigger: ", stateMachine[state].wait[1], " == ", stateMachine[state].wait[2], "\n")
		  transition = true
	       end
	    end
	    
	 else
	    transition = true
	 end
      else
	 if not quit then
	    transition = true
	 end
      end
      
      if stateMachine[state].waitFrames then
	 if uae_peek_symbol32("_hw_verticalBlankCount") > (stateMachine[state].startFrame + stateMachine[state].waitFrames) then
	    io.write("trigger: ",uae_peek_symbol32("_hw_verticalBlankCount"), " > ", stateMachine[state].startFrame, " + ", stateMachine[state].waitFrames, "\n")
	    transition = true
	 else
	    transition = false
	 end
      end
      
      if not quit and stateMachine[state].transition then
	 transition = stateMachine[state].transition(stateMachine[state])
      end
      
      if transition then
	 if stateMachine[state].exitState then
	    stateMachine[state].exitState()
	 end
	 
	 if not quit then 
	    if  stateMachine[state].writeExit then
	       Write(stateMachine[state].writeExit[1], stateMachine[state].writeExit[2])
	    end
	    
	    if stateMachine[state].next then
	       io.write("state: ", state, " -> ", stateMachine[state].next, "\n")
	       state = stateMachine[state].next
	       EnterState(stateMachine, state)
	    else
	       test = test + 1
	       if tests[test] then
		  io.write("=== next test: ", tests[test][2], " ===\nstate: ", state, " -> ", "booting\n")
		  state = "booting"
		  EnterState(tests[test][1], state)
	       end
	    end
	 end
      end	 
      
      io.flush()
      
      if quit then
	 Quit()
      end
   end   
end


function on_uae_vsync()
   --io.write(uae_peek_symbol32("_hw_verticalBlankCount"), "\n");
   
   if tests[test] then 
      Tick(tests[test][1])
   elseif not quit then
      io.write("ALL TESTS PASSED!\n")
      quit = true
      Quit()
   end
end
