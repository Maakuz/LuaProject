jumpHeight = 19
groundFriction = 0.75
airFriction = 0.97

function addGravity(y)
	local gravForce = 1
	
	y = y + gravForce
	
	return y
end

function moveX(x, dir)
	local speed = 2
	
	x = x + (dir * speed)
	
	return x
end


function update()
	handleInputs()
	local x, y = getAccel()
	
	y = addGravity(y)
	
	local dir = getDir()
	
	x = moveX(x, dir)
	
	local xVel, yVel = getVelocity()

	xVel = xVel + x
	yVel = yVel + y
	
	xVel = xVel * groundFriction
	yVel = yVel * airFriction
	
	setVelocity(xVel, yVel)
	
	setPosition(xVel, yVel)
	setAccel(0, 0)
end

