#include "../include/bunnyManager.h"
#include "../include/userInput.h"
#include "../include/util.h"

BunnyManager::BunnyManager(const std::vector<std::string> &maleNames, const std::vector<std::string> &femaleNames, const std::vector<std::string> &infectedNames, const int &x, const int &y, const int &n, const bool &VERBOSE) : maleNames(maleNames), femaleNames(femaleNames), infectedNames(infectedNames), VERBOSE(VERBOSE)
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

    infectedCount = 0;
}

/**
 * @brief method that increments time for all rabits, carries out rabbit births and then spreads the infection
 */
void BunnyManager::progressTime()
{
    // due to nature of list oldest naturally come to the front
    std::list<std::shared_ptr<Bunny>> females = std::list<std::shared_ptr<Bunny>>();
    std::list<std::shared_ptr<Bunny>> infected = std::list<std::shared_ptr<Bunny>>();

    // age rabbits
    ageBunnies(females, infected);

    // breed the rabbits
    int born = 0;
    breedBunnies(females, born);

    // spread the infection
    int newInfections = 0;
    spreadInfection(infected, newInfections);

    infectedCount = infected.size(); // used to increment this while iterating
    std::cout << std::endl
              << "Born: " << born << " Turned: " << newInfections << std::endl;
    std::cout << "Currently: " << bunnies.size() - infectedCount << " healthy rabbits " << std::endl;
    std::cout << "Currently: " << infectedCount << " infected " << std::endl;
}

void BunnyManager::ageBunnies(std::list<std::shared_ptr<Bunny>> &femaleBunnies, std::list<std::shared_ptr<Bunny>> &infectedBunnies)
{
    auto it = bunnies.begin();
    for (; it != bunnies.end(); ++it)
    {
        std::shared_ptr<Bunny> bunny = *it;
        bunny->increaseAge();
        if (bunny->isDead())
        {
            std::cout << bunny->getName() << " has died of old age " << std::endl;
            // remove from list
            bunnies.erase(it--);
        }
        else
        {
            // move to a random direction
            std::pair<int, int> newPos = getFreeSpace(bunny->getPosition());       // if it cant move this will return the original position
            grid[bunny->getPosition().first][bunny->getPosition().second].reset(); // reset the weak ptr
            bunny->setPosition(newPos);
            grid[newPos.first][newPos.second] = bunny; // assign weak ptr to observe this shared ptr
            if (VERBOSE)
                std::cout << ColourUtility::getColourString(bunny->getColour()) << " " << (bunny->getSex() ? "Female " : "Male ") << bunny->getName() << " is now " << bunny->getAge() << std::endl;
            if (!bunny->isInfected() && (bunny->getSex() && bunny->getAge() > 1))
            {
                femaleBunnies.push_back(bunny);
            }
            if (bunny->isInfected())
                infectedBunnies.push_back(bunny);
        }
    }
    if (VERBOSE)
    {
        std::cout << std::endl;
        sleep(1);
    }
}

/**
 * @brief
 *
 * @param females
 * @param born
 */
void BunnyManager::breedBunnies(const std::list<std::shared_ptr<Bunny>> &females, int &born)
{
    bool oldMale = std::any_of(bunnies.begin(), bunnies.end(), [](const std::shared_ptr<Bunny> &it)
                               { return (*it).getAge() >= 2 && (*it).getSex() == 0; });

    // iterate through list of females
    for (auto &it : females)
    {
        // check if there is an eligible male within one tile of the female
        if (PROXIMITY_BREEDING && !proximityFertileMale(it->getPosition()) || !PROXIMITY_BREEDING && !oldMale)
            continue;

        auto child = addBunny(it.get());
        if (child != nullptr)
            ++born;
    }
    if (VERBOSE)
    {
        std::cout << std::endl;
        sleep(2);
    }
}

/**
 * @brief Spread the infection from infected bunnies to close healthy bunnies
 *
 * @param infected list of those already infected
 * @param newInfections number of new infected
 */
void BunnyManager::spreadInfection(const std::list<std::shared_ptr<Bunny>> &infected, int &newInfections)
{
    for (auto &it : infected)
    {
        // check the surrounding cells for uninfected rabbits
        std::vector<std::pair<int, int>> cells = util::validCells(it->getPosition(), grid.size(), grid[0].size());
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
}

/**
 * @brief create new bunny instances
 *
 * @param mother a pointer to the mother object this is used to determine position and colour, if null these are random
 */
std::shared_ptr<Bunny> BunnyManager::addBunny(const Bunny *mother)
{
    int random = std::rand() % 100;
    Gender sex = (random % 2 == 0) ? Gender::Male : Gender::Female;
    Colour colour = (mother == nullptr) ? Colour(random % Colour::Count) : mother->getColour();
    std::pair<int, int> pos = (mother == nullptr) ? std::pair<int, int>{std::rand() % grid.size(), std::rand() % grid[0].size()} : getFreeSpace(mother->getPosition());

    // potential issue with adding bunnies without a mother
    // it will select a random cell and then overwrite anything that is in it
    if (mother != nullptr && (pos.first == mother->getPosition().first && pos.second == mother->getPosition().second)) // no free valid spaces
        return nullptr;                                                                                                // dont add the child

    if (!grid[pos.first][pos.second].expired())
    {
        return nullptr;
    }

    bool infected = (random <= 2);
    std::string name;
    if (infected)
    {
        random = std::rand() % infectedNames.size();
        name = infectedNames[random];
        infectedCount++;
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
    if (VERBOSE)
        std::cout << ColourUtility::getColourString(colour) << " " << (sex ? "Female" : "Male") << " "
                  << ((infected) ? "Infected Bunny" : "Bunny") << " " << name << " was born!" << std::endl;
    std::shared_ptr<Bunny> sharedPtr = std::make_shared<Bunny>(sex, colour, name, 0, infected, pos);
    bunnies.emplace_back(sharedPtr);
    grid[pos.first][pos.second] = sharedPtr;
    return sharedPtr;
}

/**
 * @brief method to print the current state of the program, how many bunnies are healthy and how many are infected
 */
void BunnyManager::printState() const
{
    int healthy = (bunnies.size() - infectedCount > 0) ? bunnies.size() - infectedCount : 0;
    std::cout << "Healthy bunnies: " << healthy << std::endl;
    std::cout << "Infected bunnies: " << infectedCount << std::endl;
}

/**
 * @brief This is the main logic loop that increments culls and displays the grid
 *
 */
void BunnyManager::run()
{
    int healthy = bunnies.size();
    while (healthy > 0)
    {
        system("cls");
        progressTime();
        if (VERBOSE)
        {
            std::cout << std::endl;
            std::cout << "--------------------------" << std::endl;
            sleep(2);
            printState();
            std::cout << "--------------------------" << std::endl;
        }
        if (POPULATION_LIMIT && bunnies.size() >= 1000)
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
        healthy = bunnies.size() - infectedCount; // unsigned int was causing a rolling value
    }
    displayGrid();
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
        if ((*it)->isInfected())
            --infectedCount;

        bunnies.erase(it);
    }
    std::cout << bunnies.size() << std::endl;
    std::cin;
}

// check if there is a old male within one cell of the provided position
bool BunnyManager::proximityFertileMale(const std::pair<int, int> &pos)
{
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
    for (auto it = possibleMoves.begin(); it != possibleMoves.end(); ++it)
    {
        if (grid[it->first][it->second].expired()) // space is free
            continue;
        possibleMoves.erase(it--);
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