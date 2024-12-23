#pragma once

#include <cstdint>
#include <utility>

#include "GameMap.h"

enum class Direction : uint8_t
{
    Up,
    Down,
    Left,
    Right
};

class Player
{
private:
    std::pair<int, int> position;
    Direction getDirection = Direction::Up;
    const GameMap &gameMap;

public:
    Player(int row, int col, const GameMap &gameMap) : position(row, col), gameMap(gameMap) {}

    const std::pair<int, int> &getPosition() const
    {
        return position;
    }

    Direction direction() const
    {
        return getDirection;
    }

    bool isInsideMap() const
    {
        return gameMap.isInside(position.first, position.second);
    }

    void move()
    {
        switch (getDirection)
        {
            case Direction::Up:
                if (!gameMap.isObstacle(position.first - 1, position.second))
                {
                    --position.first;
                    return;
                }
                break;
            case Direction::Down:
                if (!gameMap.isObstacle(position.first + 1, position.second))
                {
                    ++position.first;
                    return;
                }
                break;
            case Direction::Left:
                if (!gameMap.isObstacle(position.first, position.second - 1))
                {
                    --position.second;
                    return;
                }
                break;
            case Direction::Right:
                if (!gameMap.isObstacle(position.first, position.second + 1))
                {
                    ++position.second;
                    return;
                }
                break;
        }
        rotateDirection();
    }

private:
    void rotateDirection()
    {
        switch (getDirection)
        {
            case Direction::Up:
                getDirection = Direction::Right;
                break;
            case Direction::Right:
                getDirection = Direction::Down;
                break;
            case Direction::Down:
                getDirection = Direction::Left;
                break;
            case Direction::Left:
                getDirection = Direction::Up;
                break;
        }
    }
};