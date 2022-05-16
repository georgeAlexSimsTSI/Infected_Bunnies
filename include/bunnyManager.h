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

    unsigned int infectedCount;

    // grid
    std::vector<std::vector<std::weak_ptr<Bunny>>> grid; // Weak pointers are used so that if I delete a bunny from the list I dont have to remove them from the grid

    const bool VERBOSE = false, PROXIMITY_BREEDING = false, POPULATION_LIMIT = false;

    // Method to pass time
    void progressTime();
    // create a new bunny instance, mother is used to determine colour and starting position
    std::shared_ptr<Bunny> addBunny(const Bunny *mother);
    // kill 50% of the bunnies
    void cull();
    // determine if there is a male bunny of 2 years or older in an adjacent space
    bool proximityFertileMale(const std::pair<int, int> &pos);
    // How many infected bunnies there are and how many healthy ones
    void printState() const;
    // check surronding cells for a free space then randomly select one
    std::pair<int, int> getFreeSpace(const std::pair<int, int> &pos);
    // print the grid with bunny possitions, display character is dependent on status
    void displayGrid() const;
    // utility method for the display grid
    static char getBunnyChar(const std::weak_ptr<Bunny> &bunny);
    // function to breed bunnies
    void breedBunnies(const std::list<std::shared_ptr<Bunny>> &females, int &born);
    // function to spread the infection
    void spreadInfection(const std::list<std::shared_ptr<Bunny>> &infected, int &newInfections);

    void ageBunnies(std::list<std::shared_ptr<Bunny>> &femaleBunnies, std::list<std::shared_ptr<Bunny>> &infectedBunnies);

public:
    BunnyManager() : BunnyManager({"Peter", "Thumper"}, {"Daisy", "Martha"}, {"Patient Zero"}, 20, 20, 5, false){};
    BunnyManager(const std::vector<std::string> &maleNames, const std::vector<std::string> &femaleNames, const std::vector<std::string> &infectedNames, const int &x, const int &y, const int &n, const bool &VERBOSE);

    void run(); // Main logic loop that increments all bunnies until there are no healthy bunnies
};

#endif