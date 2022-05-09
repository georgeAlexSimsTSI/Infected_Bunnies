#ifndef BUNNY_MANAGER_H
#define BUNNY_MANAGER_H

#include <iostream>
#include <string>
#include <memory>
#include <stdlib.h>
#include <list>
#include <vector>
#include <unistd.h>
#include <map>

#include "bunny.h"

/**
 * @brief class responsible for handling all logic and memory storage
 */
class BunnyManager
{
private:
    std::list<std::shared_ptr<Bunny>> bunnies; // list of shared ptr objects for Bunnies on the heap
    std::vector<std::string> maleNames;
    std::vector<std::string> femaleNames;
    std::vector<std::string> infectedNames; // this is just for fun as I like the idea of original infected having infected like names

    void increment();
    void addBunny(const Bunny *mother);
    void cull();
    bool oldMale();
    void printState() const;

public:
    BunnyManager() : BunnyManager({"Peter", "Thumper"}, {"Daisy", "Martha"}, {"Dracula", "Count Von Count", "Mitchel"}, 10){};
    BunnyManager(const std::vector<std::string> &maleNames, const std::vector<std::string> &femaleNames, const std::vector<std::string> &infectedNames, const int &startingAmount);
    void run();
};

#endif