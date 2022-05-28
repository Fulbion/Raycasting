#include "Player.hpp"

Player::Player(float i_x, float i_y) :
	direction(0),
	x(i_x), y(i_y),
	sprite(texture)
{
	texture.loadFromFile("assets/player" + std::to_string(CELL_SIZE) + ".png");
	size = texture.getSize().y;
}

void Player::setPosition(float i_x, float i_y)
{
	x = i_x;
	y = i_y;
}

void Player::drawMap2D(sf::RenderWindow& i_window)
{
	float frame = 360 / (static_cast<float>(texture.getSize().x) / texture.getSize().y);
	float shifted_direction = fmod(360 + fmod(direction + 0.5f * frame, 360), 360);

	float xStart = x + 0.5f * CELL_SIZE;
	float yStart = y + 0.5f * CELL_SIZE;

	sf::VertexArray fovVisualisation(sf::TriangleFan, SCREEN_WIDTH + 1);
	fovVisualisation[0].position = sf::Vector2f(xStart, yStart);

	sf::RectangleShape background(sf::Vector2f(MAP_WIDTH * CELL_SIZE, MAP_HEIGHT * CELL_SIZE));
	background.setFillColor(sf::Color(0, 0, 0));

	sprite.setPosition(x, y);
	sprite.setTextureRect(sf::IntRect(size * floor(shifted_direction / frame), 0, size, size));
	
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		float rayDirection = getDegrees(direction + FOV * (floor(0.5 * SCREEN_WIDTH) - 1 - i) / (SCREEN_WIDTH - 1));
		fovVisualisation[i + 1].position = sf::Vector2f(xStart + viewRays[i] * cos(degToRad(rayDirection)), yStart - viewRays[i] * sin(degToRad(rayDirection)));
	}

	i_window.draw(background);
	i_window.draw(fovVisualisation);
	i_window.draw(sprite);
}

void Player::draw(sf::RenderWindow& i_window, bool fishEye)
{
	float distance = 0.5 * CELL_SIZE / tan(degToRad(0.5 * FOV_3D));
	float wallHeight;

	int currentColumn;
	int nextColumn = SCREEN_WIDTH;
	int previousColumn = SHRT_MIN;

	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		float rayDirection = FOV * (floor(0.5 * SCREEN_WIDTH) - i) / (SCREEN_WIDTH - 1);
		float rayProjection = 0.5 * tan(degToRad(rayDirection)) / tan(degToRad(0.5 * FOV));

		if (fishEye)
		{
			wallHeight = round(SCREEN_HEIGHT * distance / viewRays[i]);

			sf::RectangleShape wall(sf::Vector2f(1, wallHeight));
			wall.setFillColor(sf::Color(0, 255 * (1 - viewRays[i] / RENDER_DISTANCE), 0));
			wall.setPosition(i, 0.5 * (SCREEN_HEIGHT - wallHeight));

			i_window.draw(wall);
		}

		else
		{
			currentColumn = round((0.5 - rayProjection) * SCREEN_WIDTH);
			
			
			if (i < SCREEN_WIDTH - 1)
			{
				float nextRayDirection = FOV * (floor(0.5 * SCREEN_WIDTH) - 1 - i) / (SCREEN_WIDTH - 1);
				rayProjection = 0.5 * tan(degToRad(nextRayDirection)) / tan(degToRad(0.5 * FOV));
				nextColumn = round((0.5 - rayProjection) * SCREEN_WIDTH);
			}

			if (previousColumn < currentColumn)
			{
				wallHeight = round(SCREEN_HEIGHT * distance / (viewRays[i] * cos(degToRad(rayDirection))));
				
				sf::RectangleShape wall(sf::Vector2f(std::max(1, nextColumn - currentColumn), wallHeight));
				wall.setFillColor(sf::Color(0, 255 * (1 - viewRays[i] / RENDER_DISTANCE), 0));
				wall.setPosition(currentColumn, 0.5 * (SCREEN_HEIGHT - wallHeight));

				i_window.draw(wall);
			}
		}
	}
}

void Player::update(std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> i_map)
{
	int xStep = 0;
	int yStep = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		direction += 5.625;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		direction -= 5.625;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		xStep += 2 * cos(degToRad(direction));
		yStep -= 2 * sin(degToRad(direction));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		xStep -= 2 * cos(degToRad(direction));
		yStep += 2 * sin(degToRad(direction));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		xStep += 2 * cos(degToRad(direction + 90));
		yStep -= 2 * sin(degToRad(direction + 90));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		xStep -= 2 * cos(degToRad(direction + 90));
		yStep += 2 * sin(degToRad(direction + 90));
	}

	if (!mapCollision(x + xStep, y + yStep, i_map))
	{
		x += xStep;
		y += yStep;
	}

	else if (!mapCollision(x + xStep, y, i_map))
	{
		x += xStep;
	}

	else if (!mapCollision(x, y + yStep, i_map))
	{
		y += yStep;
	}

	direction = getDegrees(direction);

	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		int cellStepX = 0;
		int cellStepY = 0;

		float rayDirection = getDegrees(direction + FOV * (floor(0.5 * SCREEN_WIDTH) - i) / (SCREEN_WIDTH - 1));
		
		float rayDirectionX = cos(degToRad(rayDirection));
		float rayDirectionY = -sin(degToRad(rayDirection));
		
		float rayLength = 0;
		float xStart = x + 0.5 * CELL_SIZE;
		float yStart = y + 0.5 * CELL_SIZE;

		float rayLengthX = 0;
		float rayLengthY = 0;

		float rayUnitX = CELL_SIZE * sqrt(1 + pow(rayDirectionY / rayDirectionX, 2));
		float rayUnitY = CELL_SIZE * sqrt(1 + pow(rayDirectionX / rayDirectionY, 2));

		int currentCellX = floor(xStart / CELL_SIZE);
		int currentCellY = floor(yStart / CELL_SIZE);

		if (rayDirectionX < 0)
		{
			cellStepX = -1;
			rayLengthX = rayUnitX * (xStart / CELL_SIZE - currentCellX);
		}

		else if (rayDirectionX > 0)
		{
			cellStepX = 1;
			rayLengthX = rayUnitX * (currentCellX + 1 - xStart / CELL_SIZE);
		}

		else
		{
			cellStepX = 0;
		}

		if (rayDirectionY < 0)
		{
			cellStepY = -1;
			rayLengthY = rayUnitY * (yStart / CELL_SIZE - currentCellY);
		}

		else if (rayDirectionY > 0)
		{
			cellStepY = 1;
			rayLengthY = rayUnitY * (currentCellY + 1 - yStart / CELL_SIZE);
		}

		else
		{
			cellStepY = 0;
		}

		while (rayLength < RENDER_DISTANCE)
		{
			bool isCorner = false;

			if (rayLengthX < rayLengthY)
			{
				rayLength = rayLengthX;
				rayLengthX += rayUnitX;
				currentCellX += cellStepX;
			}

			else if (rayLengthX > rayLengthY)
			{
				rayLength = rayLengthY;
				rayLengthY += rayUnitY;
				currentCellY += cellStepY;
			}

			else
			{
				isCorner = true;

				rayLength = rayLengthX;
				rayLengthX += rayUnitX;
				rayLengthY += rayUnitY;
				currentCellX += cellStepX;
				currentCellY += cellStepY;
			}

			if (0 <= currentCellX && 0 <= currentCellY && MAP_HEIGHT > currentCellY && MAP_WIDTH > currentCellX)
			{
				if (i_map[currentCellX][currentCellY] == Cell::Wall)
					break;

				else if (isCorner)
					if (i_map[currentCellX - cellStepX][currentCellY] == Cell::Wall && i_map[currentCellX][currentCellY - cellStepY] == Cell::Wall)
						break;
			}
		}

		rayLength = std::min(RENDER_DISTANCE, rayLength);
		viewRays[i] = rayLength;
	}
}