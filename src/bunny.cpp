#include "../include/bunny.h"

unsigned int Bunny::infectedCount;
unsigned int Bunny::maleCount;
unsigned int Bunny::femaleCount;

Gender Bunny::getSex() const { return this->sex; }

Colour Bunny::getColour() const { return this->colour; }

std::string Bunny::getName() const
{
    return (this->infected) ? "Radioactive Mutant Infected Bunny " + this->name : "Bunny " + this->name;
}

unsigned int Bunny::getAge() const { return this->age; }

std::pair<int, int> Bunny::getPosition() const { return this->position; }

bool Bunny::isInfected() const { return this->infected; }

void Bunny::turnInfected() { this->infected = true; }

bool Bunny::increment()
{
    // decided to have their age increment first
    ++age;
    if (infected)
    {
        if (age > 50)
            return true;
        ++infectedCount;
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

void Bunny::setPosition(const std::pair<int, int> &pos) { this->position = pos; }