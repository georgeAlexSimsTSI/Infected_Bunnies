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

void BunnyManager::increment()
{
    Bunny::vampCount = 0;
    Bunny::maleCount = 0; // only counts eligible bunnies
    Bunny::femaleCount = 0;
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
        // check the surronding cells for uninfected rabbits
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
    }

    std::cout << std::endl
              << "Born: " << born << " Turned: " << newInfections << std::endl;
    std::cout << "Currently: " << bunnies.size() << " healthy rabbits " << std::endl;
    std::cout << "Currently: " << Bunny::vampCount << " infected " << std::endl;
}

void BunnyManager::addBunny(const Bunny *mother)
{
    int random = std::rand() % 100;
    Gender sex = (random % 2 == 0) ? Gender::Male : Gender::Female;
    Colour colour = (mother == nullptr) ? Colour(random % Colour::Count) : mother->getColour();
    std::pair<int, int> pos = (mother == nullptr) ? std::pair<int, int>{std::rand() % grid.size(), std::rand() % grid[0].size()} : getFreeSpace(mother->getPosition());

    if (pos.first == -1 || pos.second == -1) // no free valid spaces
        return;                              // dont add the child

    bool infected = (random <= 2);
    std::string name;
    if (infected)
    {
        random = std::rand() % infectedNames.size();
        name = infectedNames[random];
        Bunny::vampCount++;
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
                  << ((infected) ? "Radioactive Mutant Infected Bunny" : "Bunny") << " " << name << " was born!" << std::endl;
    std::shared_ptr<Bunny> shardPtr = std::make_shared<Bunny>(sex, colour, name, 0, infected, pos);
    bunnies.emplace_back(shardPtr);
    grid[pos.first][pos.second] = shardPtr;
}

void BunnyManager::printState() const
{
    int healthy = (bunnies.size() - Bunny::vampCount > 0) ? bunnies.size() - Bunny::vampCount : 0;
    std::cout << "Healthy bunnies: " << healthy << std::endl;
    std::cout << "Infected bunnies: " << Bunny::vampCount << std::endl;
}

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
            std::cout << "User has initiated a cull " << std::endl;
            cull();
            sleep(2);
        }
        sleep(3);
        std::cout.flush();
        healthy = bunnies.size() - Bunny::vampCount; // unsigned int was causing a rolling value
    } while (healthy > 0);
    std::cout << std::endl
              << "There are no living bunnies " << std::endl;
    std::cout << "There are " << bunnies.size() << " Infected " << std::endl;
}

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
        return pos;
    return possibleMoves[std::rand() % possibleMoves.size()];
}

void BunnyManager::displayGrid()
{
    for (int i = 0; i < (grid[0].size() * 2) + 1; ++i)
        std::cout << "-";
    std::cout << std::endl;
    for (const auto &i : grid)
    {
        std::cout << "|";
        for (const auto &j : i)
        {
            std::cout << getBunnyChar(j) << "|";
        }
        std::cout << std::endl;
        for (int k = 0; k < (grid[0].size() * 2) + 1; ++k)
            std::cout << "-";
        std::cout << std::endl;
    }
}

char BunnyManager::getBunnyChar(const std::weak_ptr<Bunny> &bunny)
{
    std::shared_ptr<Bunny> sharedPtr;
    if (!(sharedPtr = bunny.lock()))
        return ' ';

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