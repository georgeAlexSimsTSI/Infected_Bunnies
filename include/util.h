#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H
#include <utility>
#include <unistd.h>
#include <vector>
#include <map>
#include <string>

namespace util
{
    std::vector<std::pair<int, int>> getSurroundingSpaces(const std::pair<int, int> &pos);
    std::vector<std::pair<int, int>> validCells(const std::pair<int, int> &pos, int boundsX, int boundsY);
}

enum Colour
{
    White,
    Grey,
    Brown,
    Black,
    Count // THIS MUST REMAIN AS THE LAST ENUM
};

namespace ColourUtility
{
    std::string getColourString(const Colour &c);
}

#endif