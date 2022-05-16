#ifndef COLOUR_H
#define COLOUR_H

#include <string>
#include <map>

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