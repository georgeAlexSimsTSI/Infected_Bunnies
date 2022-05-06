#include "../include/bunnyManager.h"
#include "../include/userInput.h"

inline std::string getColourString(const Colour &c)
{
    switch (c)
    {
    case 0:
        return "White";
    case 1:
        return "Grey";
    case 2:
        return "Brown";
    default:
        return "Black";
    }
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
    if ((bunnies.size() - Bunny::vampCount) / 2 <= Bunny::vampCount) // if over half of all bunnies are infecteds then they should all die
    {
        for (auto &bun : bunnies)
        {
            if ((*bun).isInfected())
                continue;
            std::cout << (*bun).getName() << " has been infected " << std::endl;
            ++Bunny::vampCount;
            (*bun).turnInfected();
            newInfections++;
        }
        return;
    }

    // Hacky soloution
    int count = Bunny::vampCount;
    for (int i = 0; (i < count) && (bunnies.size() - Bunny::vampCount - 1 > 0); ++i)
    {
        it = bunnies.begin();
        int random = std::rand() % (bunnies.size() - Bunny::vampCount - 1);
        for (int j = 0; j <= random;)
        {
            ++it;
            if ((*it)->isInfected())
            {
                continue;
            }
            ++j;
        }
        std::cout << (*it)->getName() << " has been turned into a infected " << std::endl;
        ++Bunny::vampCount;
        (*it)->turnInfected();
        newInfections++;
    }
    std::cout << std::endl
              << "Born: " << born << " Turned: " << newInfections << std::endl;
    std::cout << "Currently: " << bunnies.size() << " healthy rabbits " << std::endl;
    std::cout << "Currently: " << Bunny::vampCount << " infecteds " << std::endl;
}

void BunnyManager::addBunny(const Bunny *mother)
{
    int random = std::rand() % 100;
    Gender sex = (random % 2 == 0) ? Gender::Male : Gender::Female;
    Colour colour = (mother == nullptr) ? Colour(random % Colour::Count) : mother->getColour();
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
    std::cout << getColourString(colour) << " " << (sex ? "Female" : "Male") << " "
              << ((infected) ? "    Infected Bunny" : "Bunny") << " " << name << " was born!" << std::endl;
    bunnies.emplace_back(std::make_unique<Bunny>(sex, colour, name, 0, infected));
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
        healthy = bunnies.size() - Bunny::vampCount; // unsigned int was causing a rolling value
        sleep(1);
    } while (healthy > 0);
    std::cout << std::endl
              << "There are no living bunnies " << std::endl;
    std::cout << "There are " << bunnies.size() << " Infecteds " << std::endl;
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

bool BunnyManager::oldMale()
{
    return (Bunny::maleCount > 0) || std::any_of(bunnies.begin(), bunnies.end(), [](const std::shared_ptr<Bunny> &it)
                                                 { return (*it).getAge() >= 2 && (*it).getSex() == 0; });
}