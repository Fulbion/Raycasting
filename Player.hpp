#pragma once
#include <array>
#include <math.h>
#include <SFML/Graphics.hpp>

#include "global.hpp"
#include "getFPS.hpp"
#include "degToRad.hpp"
#include "getDegrees.hpp"
#include "mapCollision.hpp"

class Player
{
private:
	float direction;
	float x, y;

	float FOV = CONST_FOV;
	float FOV_3D = CONST_FOV_3D;
	float RENDER_DISTANCE = CONST_RENDER_DISTANCE;

	int size;
	std::array<float, SCREEN_WIDTH> viewRays;

	sf::Texture texture;
	sf::Sprite sprite;

public:
	Player(float i_x, float i_y);

	void setPosition(float i_x, float i_y);
	void displayDebugInfo(sf::RenderWindow& i_window, bool fishEye);
	void drawMap2D(sf::RenderWindow& i_window);
	void draw(sf::RenderWindow& i_window, bool fishEye);
	void update(std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> i_map);
};

