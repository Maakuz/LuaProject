
#include "Game.h"
//KAPITEL 25 LUA DEBUGGING
//Kanske hitta på ett sätt att använda DT
//velocity behöver nog aldrig vara på c++

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	//Window
	sf::RenderWindow window(sf::VideoMode(1280, 768), "Luaspel.exe", sf::Style::Close);

	Game game(&window);

	window.setFramerateLimit(60);

	float deltaTime;
	sf::Clock time;


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

		}

		deltaTime = time.restart().asSeconds();

		//std::cout << deltaTime * 3600.f<< "\n";

		//Hittade inget bra sätt att göra detta i game utan att dt blev typ 500s


		if (window.hasFocus())
			game.update(deltaTime);

		window.clear(sf::Color(0, 0, 0));
		window.draw(game);
		window.display();
	}


	return 0;
}