whatLevel = 0
currentLevel = ""

function initialize()
	currentLevel = getCurrentLevel()
	loadLevel(currentLevel)
end

function selectLevel()
	currentLevel = io.read()
	
	loadLevel(currentLevel)
end


function update(deltaTime, gameState)
	val, msg = pcall(handleInput)

	if gameState == 0 then 
		updateGame(deltaTime)
	
	elseif gameState == 3 then 
		updateDeath(deltaTime)
	
	elseif gameState == 4 then 
		updateGameOver(deltaTime)
	
	elseif gameState == 5 then
		stopMusic()
		updateGameWon(deltaTime)
		end
end

function updateGame(deltaTime)
	handleCollision(deltaTime)
	checkAlive()
	updateViewPort()
	updateUI()

	test = hitGoal()
	if test == true then
		whatLevel = whatLevel + 1
		currentLevel = getCurrentLevel()
		
		if currentLevel == "victory" then 
			setDummyColor(0, 0, 0, 0)
			initializeGameWon()
			
		else 
			loadLevel(currentLevel) 
			end
		
		end
		
end

function updateDeath(deltaTime)
	local color = {255, 0, 0, 50}

	if getLives() > 0 then 
		setDummyColor(color[1], color[2], color[3], color[4])
		
		if waitForInput() then 
			loadLevel(currentLevel) 
			end
		
	else
		setDummyColor(0, 0, 0, 0)
		initializeGameOver()
		end
end

function updateGameOver(deltaTime)
	updateGameOverEvent(deltaTime)
end

function updateGameWon(deltaTime) -- TODO: Get a new event for winning
	updateGameOverEvent(deltaTime)
end

