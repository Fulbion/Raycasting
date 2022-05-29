#pragma once

constexpr float PI = 3.141592653589793f;

constexpr float CONST_FOV = 60.f;
constexpr float CONST_FOV_3D = 60.f;
constexpr float CONST_RENDER_DISTANCE = 96;

constexpr unsigned int CELL_SIZE = 8;
constexpr unsigned int MAP_CELL_SIZE = 8;
constexpr unsigned int MAP_WIDTH = 40;
constexpr unsigned int MAP_HEIGHT = 24;
constexpr unsigned int SCREEN_RESIZE = 1;

constexpr unsigned int SCREEN_HEIGHT = 540;
constexpr unsigned int SCREEN_WIDTH = 960;

enum Cell
{
	Empty,
	Wall
};