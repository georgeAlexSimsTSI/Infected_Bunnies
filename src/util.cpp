#include "../include/util.h"

namespace util
{
    /**
     * @brief return a vector of positions surrounding the given coordinates
     *
     * @param pos coordinate position
     * @return std::vector<std::pair<int, int>> vector of coordinates
     */
    std::vector<std::pair<int, int>> getSurroundingSpaces(const std::pair<int, int> &pos)
    {
        int first = pos.first, second = pos.second;
        return {{first + 1, second}, {first - 1, second}, {first, second + 1}, {first, second - 1}, {first + 1, second - 1}, {first + 1, second + 1}, {first - 1, second + 1}, {first - 1, second - 1}};
        ;
    }

    /**
     * @brief returns a vector of positions that are within the specified bounds and adjacent to a given point
     *
     * @param pos the position we are looking for adjacent spaces to
     * @param boundsX upper bounds of  x
     * @param boundsY upper bounds of y
     * @return std::vector<std::pair<int, int>> vector of valid positions
     */
    std::vector<std::pair<int, int>> validCells(const std::pair<int, int> &pos, int boundsX, int boundsY)
    {
        std::vector<std::pair<int, int>> possiblePositions = getSurroundingSpaces(pos);
        std::vector<std::pair<int, int>> validDirections;

        for (const auto &i : possiblePositions)
        {
            if (i.first < 0 || i.first >= boundsX || i.second < 0 || i.second >= boundsY)
                continue;
            validDirections.push_back(i);
        }
        return validDirections;
    }
}