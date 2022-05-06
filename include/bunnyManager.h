#ifndef BUNNY_MANAGER_H
#define BUNNY_MANAGER_H

#include <iostream>
#include <string>
#include <memory>
#include <stdlib.h>
#include <list>
#include <vector>
#include <unistd.h>

// used for shuffle
#include <chrono>
#include <random>

#include "bunny.h"

class BunnyManager
{
private:
    std::list<std::shared_ptr<Bunny>> bunnies;
    std::vector<std::string> maleNames;
    std::vector<std::string> femaleNames;
    std::vector<std::string> infectedNames; // this is just for fun as I like the idea of original infected having distinct names

    // grid
    std::vector<std::vector<std::weak_ptr<Bunny>>> grid;

    bool verbose = false;

    BunnyManager() : BunnyManager({"Peter", "Thumper"}, {"Daisy", "Martha"}, {"Patient Zero"}, 20, 20, 5, false){};
    BunnyManager(const std::vector<std::string> &maleNames, const std::vector<std::string> &femaleNames, const std::vector<std::string> &infectedNames, const int &x, const int &y, const int &n, const bool &verbose);
    void increment();
    void addBunny(const Bunny *mother);
    void cull();
    bool oldMale(const std::pair<int, int> &pos);
    void printState() const;
    std::pair<int, int> getFreeSpace(const std::pair<int, int> &pos);
    void displayGrid();
    static char getBunnyChar(const std::weak_ptr<Bunny> &bunny);

public:
    void run();
};

#endif