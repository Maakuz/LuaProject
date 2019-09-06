path = "../Res/Levels/"
filename = "Level1.txt"

levels = {}
nrOfLevels = 0

function findLevels()
	nrOfLevels = 0

	for dir in io.popen([[dir "../res/Levels/" /b]]):lines() do
		levels[nrOfLevels] = dir
		nrOfLevels = nrOfLevels + 1;
	end
end

function getFileName(index)
	ret = levels[index]
	
	if ret == nil then ret = "victory" end
	
	return ret
end

function getDimensions()
	
	local x = 0
	local y = 0
	
	val, msg = pcall(io.input, path .. filename)

	print(msg)
	
	x = io.read("*number")
	y = io.read("*number")
	
		
	return x, y
end

function setFileName(name)
	filename = name
end

function getTileMap()
	val, msg = pcall(io.input, path .. filename)
	
	--row 3 is where the texture path is
	io.read()
	io.read()
	return io.read()
end

function getLevelData()
	
	local rows = getRows()
	local layer1 = ""
	local layer2 = ""
	local layer3 = ""
	local layer4 = ""
	local layer5 = ""
	
	file = io.open(path .. filename, "r")
	
	io.input(file)
	
	while io.read() ~= "[layer0]" do 
	end
	
	for i = 1, rows do
			layer1 = layer1 .. io.read()
			end
		
		while io.read() ~= "[layer1]" do 
	end
	
	for i = 1, rows do
			layer2 = layer2 .. io.read()
			end
			
			while io.read() ~= "[layer2]" do 
	end
	
	for i = 1, rows do
			layer3 = layer3 .. io.read()
			end
			
			while io.read() ~= "[layer3]" do 
	end
	
	for i = 1, rows do
			layer4 = layer4 .. io.read()
			end
			
			while io.read() ~= "[layer4]" do 
	end
	
	for i = 1, rows do
			layer5 = layer5 .. io.read()
			end
	io.close(file)

	return layer1, layer2, layer3, layer4, layer5
end