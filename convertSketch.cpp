#include "convertSketch.hpp"

std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> convertSketch(Player& i_player)
{
    std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> outputMap{};

    sf::Image mapSketch;
    mapSketch.loadFromFile("assets/mapSketch.png");

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            outputMap[x][y] = Cell::Empty;
            sf::Color pixel = mapSketch.getPixel(x, y);
            
            if (pixel == sf::Color(0, 0, 0))
                outputMap[x][y] = Cell::Wall;

            if (pixel == sf::Color(255, 0, 0))
                i_player.setPosition(CELL_SIZE * x, CELL_SIZE * y);
        }
    }

    return outputMap;
}
