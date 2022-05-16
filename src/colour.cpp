#include "../include/colour.h"

namespace ColourUtility
{
    /**
     * @brief get the corresponding string to a colour enum
     *
     * @param c the colour enum
     * @return std::string a string representing the colour
     */
    std::string getColourString(const Colour &c)
    {
        std::map<Colour, std::string> colourMap{
            {Colour::White, "White"},
            {Colour::Spotted, "Spotted"},
            {Colour::Brown, "Brown"},
            {Colour::Black, "Black"}};
        return colourMap[c];
    }
}