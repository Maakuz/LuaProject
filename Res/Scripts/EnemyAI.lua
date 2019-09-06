groundFriction = 0.75
airFriction = 0.97

function addGravity(y)
	local gravForce = 0.4
	
	y = y + gravForce
	
	return y
end

function moveX(x, dir)
	local speed = 0.2
	
	x = x + (dir * speed)
	
	return x
end


function update()
	local x = 0
	local y = 0
	
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
