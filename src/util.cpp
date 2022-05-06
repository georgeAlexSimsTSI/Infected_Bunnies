#include "../include/util.h"

namespace util
{
    std::vector<std::pair<int, int>> getSurroundingSpaces(const std::pair<int, int> &pos)
    {
        int first = pos.first, second = pos.second;
        return {{first + 1, second}, {first - 1, second}, {first, second + 1}, {first, second - 1}, {first + 1, second - 1}, {first + 1, second + 1}, {first - 1, second + 1}, {first - 1, second - 1}};
        ;
    }

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

namespace ColourUtility
{
    std::string getColourString(const Colour &c)
    {
        std::map<Colour, std::string> colourMap{
            {Colour::White, "White"},
            {Colour::Grey, "Grey"},
            {Colour::Brown, "Brown"},
            {Colour::Black, "Black"}};
        return colourMap[c];
    }
}