#include <array>
#include <string>
#include <math.h>
#include <SFML/Graphics.hpp>

#include "global.hpp"
#include "Player.hpp"
#include "convertSketch.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Raycasting");
	window.setFramerateLimit(30);

	sf::Texture t_mapWall; t_mapWall.loadFromFile("assets/wall" + std::to_string(CELL_SIZE) + ".png");
	sf::Sprite mapWall(t_mapWall);

	bool drawMap = true;
	bool fishEye = false;

	std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> map{};

	Player player(0, 0);
	map = convertSketch(player);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Escape)
					window.close();

				if (event.key.code == sf::Keyboard::M)
				{
					if (drawMap) drawMap = false;
					else         drawMap = true;
				}

				if (event.key.code == sf::Keyboard::F)
				{
					if (fishEye) fishEye = false;
					else         fishEye = true;
				}
			}
		}

		window.clear();

		player.update(map);
		player.draw(window, fishEye);

		if (drawMap)
		{
			player.drawMap2D(window);
			for (int x = 0; x < MAP_WIDTH; x++)
			{
				for (int y = 0; y < MAP_HEIGHT; y++)
				{
					if (map[x][y] == Cell::Wall)
					{
						mapWall.setPosition(static_cast<float>(CELL_SIZE * x), static_cast<float>(CELL_SIZE * y));
						window.draw(mapWall);
					}
				}
			}
		}

		window.display();
	}

	return 0;
}