#include "../include/bunny.h"

unsigned int Bunny::vampCount;
unsigned int Bunny::maleCount;
unsigned int Bunny::femaleCount;

Gender Bunny::getSex() const { return this->sex; }

Colour Bunny::getColour() const { return this->colour; }

std::string Bunny::getName() const
{
    return (this->vampire) ? "Radioactive Mutant Vampire Bunny " + this->name : "Bunny " + this->name;
}

unsigned int Bunny::getAge() const { return this->age; }

bool Bunny::isVampire() const { return this->vampire; }

void Bunny::turnVampire() { this->vampire = true; }

bool Bunny::increment()
{
    // decided to have their age increment first
    ++age;
    if (vampire)
    {
        if (age > 50)
            return true;
        ++vampCount;
        return false;
    }
    if (age > 10)
        return true;

    if (age >= 2)
    {
        if (sex == 0)
            ++maleCount;
        else
            ++femaleCount;
    }
    return false;
}