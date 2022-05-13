#include "../include/bunnyManager.h"
#include "../include/userInput.h"

/**
 * @brief Get the Colour String
 * @param c a Colour enum
 * @return std::string the colour string
 */
inline std::string getColourString(const Colour &c)
{
    std::map<Colour, std::string> colourMap{
        {Colour::White, "White"},
        {Colour::Spotted, "Spotted"},
        {Colour::Brown, "Brown"},
        {Colour::Black, "Black"}};
    return colourMap[c];
}

/**
 * @brief Construct a new Bunny
 * @param maleNames names of male bunnies
 * @param femaleNames names of female bunnies
 * @param infectedNames names of infected bunnies
 * @param startingAmount the amount of bunnies to create at the start
 */
BunnyManager::BunnyManager(const std::vector<std::string> &maleNames, const std::vector<std::string> &femaleNames, const std::vector<std::string> &infectedNames, const int &startingAmount) : maleNames(maleNames), femaleNames(femaleNames), infectedNames(infectedNames)
{
    static bool seeded = false;
    if (!seeded)
    {
        srand(time(NULL));
        seeded = true;
    }
    bunnies = std::list<std::shared_ptr<Bunny>>();

    for (int i = 0; i < startingAmount; ++i)
        addBunny(nullptr);
}

/**
 * @brief method that increments bunny ages, breeds them and then spreads the infection
 *
 */
void BunnyManager::increment()
{
    Bunny::infectedCount = 0;
    Bunny::maleCount = 0;   // only counts eligible bunnies
    Bunny::femaleCount = 0; // list of females that can have kids
    int born = 0;
    // due to nature of list oldest naturally come to the front
    std::list<std::shared_ptr<Bunny>>::iterator it = bunnies.begin();
    std::list<std::shared_ptr<Bunny>> females = std::list<std::shared_ptr<Bunny>>();
    for (; it != bunnies.end(); ++it)
    {
        if ((*it)->increment()) // increment returns true if the bunny is too old
        {
            std::cout << (*it)->getName() << " has died of old age " << std::endl;
            // remove from list
            bunnies.erase(it--); // erase then decrement
        }
        else
        {
            std::cout << getColourString((*it)->getColour()) << " " << ((*it)->getSex() ? "Female " : "Male ") << (*it)->getName() << " is now " << (*it)->getAge() << std::endl;
            if (!(*it)->isInfected() && ((*it)->getSex() && (*it)->getAge() > 1))
            {
                females.push_back(*it);
            }
        }
    }
    std::cout << std::endl;
    sleep(1);
    // iterate through list of females
    for (auto it2 : females)
    {
        born++;
        addBunny(it2.get());
    }
    std::cout << std::endl;
    sleep(2);
    int newInfections = 0;
    if ((bunnies.size() - Bunny::infectedCount) / 2 <= Bunny::infectedCount) // if over half of all bunnies are infected then they should all die
    {
        for (auto &bun : bunnies)
        {
            if ((*bun).isInfected())
                continue;
            std::cout << (*bun).getName() << " has been infected " << std::endl;
            ++Bunny::infectedCount;
            (*bun).turnInfected();
            newInfections++;
        }
        return;
    }
    int count = Bunny::infectedCount;
    for (int i = 0; (i < count) && (bunnies.size() - Bunny::infectedCount - 1 > 0); ++i)
    {
        it = bunnies.begin();
        int random = std::rand() % (bunnies.size() - Bunny::infectedCount - 1);
        for (int j = 0; j <= random;)
        {
            ++it;
            if ((*it)->isInfected())
            {
                continue;
            }
            ++j;
        }
        std::cout << (*it)->getName() << " has been infected " << std::endl;
        ++Bunny::infectedCount;
        (*it)->turnInfected();
        newInfections++;
    }
    std::cout << std::endl
              << "Born: " << born << " Turned: " << newInfections << std::endl;
    std::cout << "Currently: " << bunnies.size() << " healthy rabbits " << std::endl;
    std::cout << "Currently: " << Bunny::infectedCount << " infected " << std::endl;
}

/**
 * @brief method to create more bunny instances
 *
 * @param mother a pointer to the bunny mother, this is used to determine the colour of the child, if null it is random
 */
void BunnyManager::addBunny(const Bunny *mother)
{
    int random = std::rand() % 100;
    Gender sex = (random % 2 == 0) ? Gender::Male : Gender::Female;
    Colour colour = (mother == nullptr) ? Colour(random % Colour::Count) : mother->getColour(); // if no mother pick random colour
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
    std::cout << getColourString(colour) << " " << (sex ? "Female" : "Male") << " "
              << ((infected) ? "Infected Bunny" : "Bunny") << " " << name << " was born!" << std::endl;
    bunnies.emplace_back(std::make_unique<Bunny>(sex, colour, name, 0, infected));
}

/**
 * @brief print out the state of the program, how many healthy bunnies there are and how many are infected
 */
void BunnyManager::printState() const
{
    int healthy = (bunnies.size() - Bunny::infectedCount > 0) ? bunnies.size() - Bunny::infectedCount : 0;
    std::cout << "Healthy bunnies: " << healthy << std::endl;
    std::cout << "Infected bunnies: " << Bunny::infectedCount << std::endl;
}

/**
 * @brief Main logic loop that increments culls then displays status
 *
 */
void BunnyManager::run()
{
    int healthy;
    do
    {
        system("cls");
        increment();
        std::cout << std::endl;
        std::cout << "--------------------------" << std::endl;
        sleep(2);
        printState();
        std::cout << "--------------------------" << std::endl;

        if (bunnies.size() >= 1000)
        {
            std::cout << std::endl;
            sleep(1);
            cull();
        }

        sleep(1);
        std::cout << std::endl
                  << "Enter k to cull: " << std::endl;
        char c = userInput::waitForCharInput(2);
        if (c == 'k' || c == 'K')
        {
            std::cout << "User has initiated a cull " << std::endl;
            cull();
        }

        std::cout.flush();
        sleep(1);
        healthy = bunnies.size() - Bunny::infectedCount; // unsigned int was causing a rolling value
        sleep(1);
    } while (healthy > 0);
    std::cout << std::endl
              << "There are no living bunnies " << std::endl;
    std::cout << "There are " << bunnies.size() << " Infected " << std::endl;
}

// Kill of half of the bunnies in the system
// both branches have a different version
void BunnyManager::cull()
{
    int amount = bunnies.size() / 2, i = 0;
    std::cout << std::endl
              << "There is a cull: " << amount << " will be culled" << std::endl;
    for (auto it = bunnies.begin(); it != --bunnies.end(); ++it) // alternate method would be to do a 50 50 check on if to kill the current rabbit or the next one
    {
        if (std::rand() % 2 != 0)
            ++it;
        it = bunnies.erase(it);
        if (i == amount)
            break;
    }
    std::cout << bunnies.size() << std::endl;
    std::cin;
}

// old leftover that is no longer used but is left here for future refernce
bool BunnyManager::oldMale()
{
    return (Bunny::maleCount > 0) || std::any_of(bunnies.begin(), bunnies.end(), [](const std::shared_ptr<Bunny> &it)
                                                 { return (*it).getAge() >= 2 && (*it).getSex() == 0; });
}