#include "../include/bunnyManager.h"
#include "../include/userInput.h"
#include "../include/util.h"

BunnyManager::BunnyManager(const std::vector<std::string> &maleNames, const std::vector<std::string> &femaleNames, const std::vector<std::string> &infectedNames, const int &x, const int &y, const int &n, const bool &verbose) : maleNames(maleNames), femaleNames(femaleNames), infectedNames(infectedNames), verbose(verbose)
{
    static bool seeded = false; // seed the random number generator
    if (!seeded)
    {
        srand(time(NULL));
        seeded = true;
    }
    bunnies = std::list<std::shared_ptr<Bunny>>();
    grid = std::vector(x, std::vector(y, std::weak_ptr<Bunny>()));
    for (int i = 0; i < n; ++i)
        addBunny(nullptr);
}

/**
 * @brief method that increments time for all rabits, carries out rabbit births and then spreads the infection
 */
void BunnyManager::increment()
{
    int born = 0;
    // due to nature of list oldest naturally come to the front
    std::list<std::shared_ptr<Bunny>>::iterator it = bunnies.begin();
    std::list<std::shared_ptr<Bunny>> females = std::list<std::shared_ptr<Bunny>>();
    std::list<std::shared_ptr<Bunny>> infected = std::list<std::shared_ptr<Bunny>>();
    for (; it != bunnies.end(); ++it)
    {
        if ((*it)->increment())
        {
            std::cout << (*it)->getName() << " has died of old age " << std::endl;
            // remove from list
            bunnies.erase(it--); // erase then decrement
        }
        else
        {
            // move to a random direction
            std::pair<int, int> newPos = getFreeSpace((*it)->getPosition());       // if it cant move this will return the original position
            grid[(*it)->getPosition().first][(*it)->getPosition().second].reset(); // reset the weak ptr
            (*it)->setPosition(newPos);
            grid[newPos.first][newPos.second] = *it; // assign weak ptr to observe this shared ptr
            if (verbose)
                std::cout << ColourUtility::getColourString((*it)->getColour()) << " " << ((*it)->getSex() ? "Female " : "Male ") << (*it)->getName() << " is now " << (*it)->getAge() << std::endl;
            if (!(*it)->isInfected() && ((*it)->getSex() && (*it)->getAge() > 1))
            {
                females.push_back(*it);
            }
            if ((*it)->isInfected())
                infected.push_back(*it);
        }
    }
    if (verbose)
    {
        std::cout << std::endl;
        sleep(1);
    }
    // iterate through list of females
    for (auto it2 : females)
    {
        // check if there is an eligible male within one tile of the female
        if (!oldMale(it2->getPosition()))
            continue;
        born++;
        addBunny(it2.get());
    }
    if (verbose)
    {
        std::cout << std::endl;
        sleep(2);
    }
    int newInfections = 0;
    for (auto it2 = infected.begin(); it2 != infected.end(); ++it2)
    {
        // check the surrounding cells for uninfected rabbits
        std::vector<std::pair<int, int>> cells = util::validCells((*it2)->getPosition(), grid.size(), grid[0].size());
        std::shared_ptr<Bunny> sharedPtr;
        for (int i = 0; i < cells.size(); ++i)
        {

            if (!(sharedPtr = grid[(cells[i]).first][(cells[i]).second].lock()) || sharedPtr->isInfected())
            {
                cells.erase(cells.begin() + i);
                i--;
            }
        }
        if (cells.size() == 0)
            continue;
        int tmp = rand() % cells.size();
        sharedPtr = grid[cells[tmp].first][cells[tmp].second].lock();
        sharedPtr->turnInfected();
        ++newInfections;
    }
    Bunny::infectedCount = infected.size(); // used to increment this while iterating
    std::cout << std::endl
              << "Born: " << born << " Turned: " << newInfections << std::endl;
    std::cout << "Currently: " << bunnies.size() - Bunny::infectedCount << " healthy rabbits " << std::endl;
    std::cout << "Currently: " << Bunny::infectedCount << " infected " << std::endl;
}

/**
 * @brief create new bunny instances
 *
 * @param mother a pointer to the mother object this is used to determine position and colour, if null these are random
 */
void BunnyManager::addBunny(const Bunny *mother)
{
    int random = std::rand() % 100;
    Gender sex = (random % 2 == 0) ? Gender::Male : Gender::Female;
    Colour colour = (mother == nullptr) ? Colour(random % Colour::Count) : mother->getColour();
    std::pair<int, int> pos = (mother == nullptr) ? std::pair<int, int>{std::rand() % grid.size(), std::rand() % grid[0].size()} : getFreeSpace(mother->getPosition());

    // potential issue with adding bunnies without a mother
    // it will select a random cell and then overwrite anything that is in it
    if (mother != nullptr && (pos.first == mother->getPosition().first || pos.second == mother->getPosition().second)) // no free valid spaces
        return;                                                                                                        // dont add the child

    bool infected = (random <= 2);
    std::string name;
    if (infected)
    {
        random = std::rand() % infectedNames.size();
        name = infectedNames[random];
        Bunny::infectedCount++;
    }
    else
    {
        if (sex)
        {
            random = rand() % femaleNames.size();
            name = femaleNames[random];
        }
        else
        {
            random = rand() % maleNames.size();
            name = maleNames[random];
        }
    }
    if (verbose)
        std::cout << ColourUtility::getColourString(colour) << " " << (sex ? "Female" : "Male") << " "
                  << ((infected) ? "Infected Bunny" : "Bunny") << " " << name << " was born!" << std::endl;
    std::shared_ptr<Bunny> shardPtr = std::make_shared<Bunny>(sex, colour, name, 0, infected, pos);
    bunnies.emplace_back(shardPtr);
    grid[pos.first][pos.second] = shardPtr;
}

/**
 * @brief method to print the current state of the program, how many bunnies are healthy and how many are infected
 */
void BunnyManager::printState() const
{
    int healthy = (bunnies.size() - Bunny::infectedCount > 0) ? bunnies.size() - Bunny::infectedCount : 0;
    std::cout << "Healthy bunnies: " << healthy << std::endl;
    std::cout << "Infected bunnies: " << Bunny::infectedCount << std::endl;
}

/**
 * @brief This is the main logic loop that increments culls and displays the grid
 *
 */
void BunnyManager::run()
{
    int healthy;
    do
    {
        system("cls");
        increment();
        if (verbose)
        {
            std::cout << std::endl;
            std::cout << "--------------------------" << std::endl;
            sleep(2);
            printState();
            std::cout << "--------------------------" << std::endl;
        }
        if (bunnies.size() >= 1000)
        {
            std::cout << std::endl;
            sleep(1);
            cull();
        }
        displayGrid();
        // sleep(1);
        std::cout << std::endl
                  << "Enter k to cull: " << std::endl;
        char c = userInput::waitForCharInput(2);
        if (c == 'k' || c == 'K')
        {
            system("cls");
            std::cout << "User has initiated a cull " << std::endl;
            cull();
            sleep(2);
            displayGrid();
        }
        sleep(3);
        std::cout.flush();
        healthy = bunnies.size() - Bunny::infectedCount; // unsigned int was causing a rolling value
    } while (healthy > 0);
    std::cout << std::endl
              << "There are no living bunnies " << std::endl;
    std::cout << "There are " << bunnies.size() << " Infected " << std::endl;
}

// delete 50% of the entries in a random manner
void BunnyManager::cull()
{
    int amount = bunnies.size() / 2;
    std::cout << std::endl
              << "There is a cull: " << amount << " will be culled" << std::endl;
    std::list<std::shared_ptr<Bunny>>::iterator it;
    for (int i = 0; i < amount; ++i)
    {
        int rand = std::rand() % bunnies.size();
        it = bunnies.begin();
        std::advance(it, rand);
        // std::cout << (*it)->getName() << " has been culled " << std::endl;
        bunnies.erase(it);
    }
    std::cout << bunnies.size() << std::endl;
    std::cin;
}

// check if there is a old male within one cell of the provided position
bool BunnyManager::oldMale(const std::pair<int, int> &pos)
{
    // return (Bunny::maleCount > 0) || std::any_of(bunnies.begin(), bunnies.end(), [](const std::shared_ptr<Bunny> &it)
    //                                              { return (*it).getAge() >= 2 && (*it).getSex() == 0; });
    std::vector<std::pair<int, int>> cellsToCheck = util::validCells(pos, grid.size(), grid[0].size());
    std::shared_ptr<Bunny> sharedPtr;
    for (const auto &it : cellsToCheck)
    {
        if (!(sharedPtr = grid[it.first][it.second].lock()))
            continue;
        if (sharedPtr->getAge() >= 2 && !sharedPtr->getSex())
        {
            return true;
        }
    }
    return false;
}

// selects a random position out of the free surrounding cells
std::pair<int, int> BunnyManager::getFreeSpace(const std::pair<int, int> &pos)
{
    std::vector<std::pair<int, int>> possibleMoves = util::validCells(pos, grid.size(), grid[0].size());
    // iterate through possible moves checking if the space is empty or not
    for (auto it = possibleMoves.begin(); it != possibleMoves.begin(); ++it)
    {
        if (grid[it->first][it->second].expired()) // space is free
            continue;
        possibleMoves.erase(it);
        it--; // need to decrement iterator after erasing
    }
    if (possibleMoves.size() == 0)
        return pos; // if no free cells return the starting cell
    return possibleMoves[std::rand() % possibleMoves.size()];
}

void BunnyManager::displayGrid() const
{
    std::cout << std::endl;
    for (const auto &i : grid)
    {
        for (const auto &j : i)
        {
            std::cout << getBunnyChar(j) << " ";
        }
        std::cout << std::endl;
    }
}

char BunnyManager::getBunnyChar(const std::weak_ptr<Bunny> &bunny)
{
    std::shared_ptr<Bunny> sharedPtr;
    if (!(sharedPtr = bunny.lock()))
        return '.';

    if (sharedPtr->getAge() < 2)
    {
        if (sharedPtr->isInfected())
            return 'x';
        if (sharedPtr->getSex())
            return 'f';
        return 'm';
    }
    if (sharedPtr->isInfected())
        return 'X';
    if (sharedPtr->getSex())
        return 'F';
    return 'M';
}