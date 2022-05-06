#ifndef BUNNY_MANAGER_H
#define BUNNY_MANAGER_H

#include <iostream>
#include <string>
#include <memory>
#include <stdlib.h>
#include <list>
#include <vector>
#include <unistd.h>

#include "bunny.h"

class BunnyManager
{
private:
    std::list<std::shared_ptr<Bunny>> bunnies;
    std::vector<std::string> maleNames;
    std::vector<std::string> femaleNames;
    std::vector<std::string> vampireNames; // this is just for fun as I like the idea of original vampires having vampire like names

public:
    BunnyManager() : BunnyManager({"Peter", "Thumper"}, {"Daisy", "Martha"}, {"Dracula", "Count Von Count", "Mitchel"}){};
    BunnyManager(const std::vector<std::string> &maleNames, const std::vector<std::string> &femaleNames, const std::vector<std::string> &vampireNames) : maleNames(maleNames), femaleNames(femaleNames), vampireNames(vampireNames)
    {
        static bool seeded = false;
        if (!seeded)
        {
            srand(time(NULL));
            seeded = true;
        }
        bunnies = std::list<std::shared_ptr<Bunny>>();
    }
    void increment();
    void addBunny(const Bunny *mother);
    void run();
    void cull();
    bool oldMale();
    void printState() const;
};

#endif