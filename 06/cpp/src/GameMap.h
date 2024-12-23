#pragma once

#include <string>
#include <utility>
#include <vector>

class GameMap
{
private:
    std::vector<std::string> map;

public:
    explicit GameMap(std::vector<std::string> &&map) : map(std::move(map)) {}

    bool isObstacle(int row, int col) const
    {
        return isInside(row, col) && map[row][col] == '#';
    }

    bool isInside(int row, int col) const
    {
        return row >= 0 && row < map.size() && col >= 0 && col < map[row].size();
    }

    std::pair<int, int> findPlayer() const
    {
        for (int row = 0; row < map.size(); ++row)
        {
            if (const auto col = map[row].find('^'); col != std::string::npos)
            {
                return {row, col};
            }
        }
        return {-1, -1};
    }
};
