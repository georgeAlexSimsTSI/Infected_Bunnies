#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H
#include <utility>
#include <unistd.h>
#include <vector>
#include <map>
#include <string>

/**
 * @brief namespace of utility methods that may be used in future
 */
namespace util
{
    std::vector<std::pair<int, int>> getSurroundingSpaces(const std::pair<int, int> &pos);
    std::vector<std::pair<int, int>> validCells(const std::pair<int, int> &pos, int boundsX, int boundsY);
}

/**
 * @brief simple enum for different colours
 */
enum Colour
{
    White,
    Brown,
    Black,
    Spotted,
    Count // THIS MUST REMAIN AS THE LAST ENUM, this is used to keep track of how many colours there are
};

/**
 * @brief namespace for colour utility methods
 */
namespace ColourUtility
{
    std::string getColourString(const Colour &c);
}

#endif