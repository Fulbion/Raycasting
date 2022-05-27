#include "mapCollision.hpp"

bool mapCollision(float i_x, float i_y, const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map)
{
    float xCell = i_x / CELL_SIZE;
    float yCell = i_y / CELL_SIZE;

	for (int i = 0; i < 4; i++)
	{
		int x = 0;
		int y = 0;

		switch (i)
		{
		case 0:
			x = floor(xCell);
			y = floor(yCell);
			break;

		case 1:
			x = ceil(xCell);
			y = floor(yCell);
			break;

		case 2:
			x = floor(xCell);
			y = ceil(yCell);
			break;

		case 3:
			x = ceil(xCell);
			y = ceil(yCell);
			break;
		}

		if (0 <= x && 0 <= y && MAP_HEIGHT > y && MAP_WIDTH > x)
			if (i_map[x][y] == Cell::Wall)
				return true;
	}

	return false;
}
