volume = 20
musicVolume = 10
gameOverVolume = 20

function playJumpSfx()
	luaPlayJump(volume - 10)
end

function playDeathSound()
	luaPlayDead(volume)
end

function playHitSound()
	luaPlayHit(volume)
end

function playFish()
	luaPlayFish(volume)
end

function playLife()
	luaPlayLife(volume + 30)
end