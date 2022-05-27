#pragma once
#include <array>
#include <SFML/Graphics.hpp>

#include "global.hpp"
#include "Player.hpp"

std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> convertSketch(Player& i_player);