#include "../include/bunny.h"

Gender Bunny::getSex() const { return this->sex; }

Colour Bunny::getColour() const { return this->colour; }

std::string Bunny::getName() const { return (this->infected) ? "Infected Bunny " + this->name : "Bunny " + this->name; }

unsigned int Bunny::getAge() const { return this->age; }

std::pair<int, int> Bunny::getPosition() const { return this->position; }

bool Bunny::isInfected() const { return this->infected; }

void Bunny::turnInfected() { this->infected = true; }

/**
 * @brief increment the bunny age
 *
 * @return true if the rabbit dies of old age
 * @return false if the rabbit is still alive
 */
bool Bunny::increaseAge()
{
    // decided to have their age increment first
    ++age;
    if (infected)
    {
        if (age > 50)
            return true;
        return false;
    }
    if (age > 10)
        return true;

    return false;
}

void Bunny::setPosition(const std::pair<int, int> &pos) { this->position = pos; }