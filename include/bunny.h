#ifndef BUNNY_H
#define BUNNY_H

#include <string>
#include <iostream>
enum Gender
{
    Male,
    Female
};
enum Colour
{
    White,
    Grey,
    Brown,
    Black,
    Count //THIS MUST REMAIN AS THE LAST ENUM
};

class Bunny
{
private:
    Gender sex;
    Colour colour;
    std::string name;
    unsigned int age;
    bool vampire;

public:
    // These are variables used for counting, when the manager progresses the year these will be set back to zero and then increment
    static unsigned int vampCount;
    static unsigned int maleCount;
    static unsigned int femaleCount;

    Bunny() : Bunny(Gender::Male, Colour::Grey, std::string("Thumper"), 0, false){};
    Bunny(const Gender &sex, const Colour &colour, const std::string &name, const unsigned int &age, const bool &vampire) : sex(sex), colour(colour), name(name), age(age), vampire(vampire){};
    // ~Bunny() { std::cout << "Bunny: " << name << " has been deconstructed " << std::endl; }

    Gender getSex() const;
    Colour getColour() const;
    std::string getName() const;
    unsigned int getAge() const;
    bool isVampire() const;

    void turnVampire();
    bool increment(); // if the bunny dies of old age it will return true
};

#endif