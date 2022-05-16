#ifndef BUNNY_H
#define BUNNY_H

#include <string>
#include <iostream>
#include "colour.h"
#include "gender.h"

/**
 * @brief class responsible for representing bunnies
 */
class Bunny
{
private:
    Gender sex;
    Colour colour;
    std::string name;
    unsigned int age;
    bool infected;
    std::pair<int, int> position;

public:
    Bunny() : Bunny(Gender::Male, Colour::White, std::string("Thumper"), 0, false){};
    Bunny(const Gender &sex, const Colour &colour, const std::string &name, const unsigned int &age, const bool &infected) : Bunny(sex, colour, name, age, infected, std::make_pair(0, 0)){};
    Bunny(const Gender &sex, const Colour &colour, const std::string &name, const unsigned int &age, const bool &infected, const std::pair<int, int> &position) : sex(sex), colour(colour), name(name), age(age), infected(infected), position(position){};
    // ~Bunny() { std::cout << "Bunny: " << name << " has been deconstructed " << std::endl; }

    Gender getSex() const;
    Colour getColour() const;
    std::string getName() const;
    std::pair<int, int> getPosition() const;
    unsigned int getAge() const;
    bool isInfected() const;

    void turnInfected();
    bool increaseAge(); // if the bunny dies of old age it will return true
    void setPosition(const std::pair<int, int> &pos);
};

#endif