#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
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
    std::vector<std::string> gameMap((std::istream_iterator<std::string>(inputFile)),
                                     std::istream_iterator<std::string>());

    const GameMap map(std::move(gameMap));
    const auto [row, col] = map.findPlayer();
    Player player(row, col, map);

    std::set<std::tuple<int, int>> visited;
    while (player.isInsideMap())
    {
        visited.insert(player.getPosition());
        player.move();
    }

    std::cout << "Unique visited places: " << visited.size() << "\n";
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