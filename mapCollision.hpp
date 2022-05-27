#pragma once
#include <array>
#include "global.hpp"

bool mapCollision(float i_x, float i_y, const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map);