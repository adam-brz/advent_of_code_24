#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "GameMap.h"
#include "Player.h"

int main(int argc, char **argv)
try
{
    if (argc < 2)
    {
        std::cerr << "Input data is missing\n"
                  << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    const std::vector<std::string> gameMap((std::istream_iterator<std::string>(inputFile)),
                                           std::istream_iterator<std::string>());

    const auto isPlaceForObstacle = [&gameMap](const int row, const int col)
    {
        const GameMap map(
            [&]()
            {
                auto mapCopy = gameMap;
                mapCopy[row][col] = '#';
                return mapCopy;
            }());

        const auto [playerRow, playerCol] = map.findPlayer();
        Player player(playerRow, playerCol, map);

        std::set<std::tuple<int, int, Direction>> visited;

        while (player.isInsideMap())
        {
            const auto [row, col] = player.getPosition();
            const auto key = std::make_tuple(row, col, player.direction());

            if (visited.contains(key))
            {
                return true;
            }

            visited.insert(key);
            player.move();
        }

        return false;
    };

    int possiblePlacesForObstacles = 0;
    int progress = 0;
    for (int row = 0; row < gameMap.size(); ++row)
    {
        for (int col = 0; col < gameMap[row].size(); ++col)
        {
            std::cout << "Progress: " << ++progress << "/" << gameMap.size() * gameMap[row].size() << "\r"
                      << std::flush;
            if (gameMap[row][col] == '.')
            {
                possiblePlacesForObstacles += isPlaceForObstacle(row, col);
            }
        }
    }

    std::cout << "Number of possible places: " << possiblePlacesForObstacles << '\n';
    return 0;
}
catch (const std::exception &e)
{
    std::cerr << "Exception: " << e.what() << "\n";
    return 1;
}
catch (...)
{
    std::cerr << "Unknown exception\n";
    return 1;
}