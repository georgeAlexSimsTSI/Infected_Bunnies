#include "../include/bunnyManager.h"

void BunnyManager::increment()
{
    Bunny::vampCount = 0;
    Bunny::maleCount = 0; // only counts eligible bunnies
    Bunny::femaleCount = 0;
    int born = 0;
    // due to nature of list oldest naturally come to the front excluding vampires
    std::list<std::shared_ptr<Bunny>>::iterator it = bunnies.begin();
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
            std::cout << (*it)->getName() << " is now " << (*it)->getAge() << std::endl;

            if ((*it)->getSex() && (*it)->getAge() > 1 && oldMale())
            {
                addBunny(it->get());
                ++born;
            }
        }
    }

    int turnedVamps = 0;
    if ((bunnies.size() - Bunny::vampCount) / 2 <= Bunny::vampCount) // if over half of all bunnies are vampires then they should all die
    {
        for (auto &bun : bunnies)
        {
            if ((*bun).isVampire())
                continue;
            std::cout << (*bun).getName() << " has been turned into a vampire " << std::endl;
            ++Bunny::vampCount;
            (*bun).turnVampire();
            turnedVamps++;
        }
        return;
    }

    // Hacky soloution                      kill me
    int count = Bunny::vampCount;
    for (int i = 0; (i < count) && (bunnies.size() - Bunny::vampCount - 1 > 0); ++i)
    {
        it = bunnies.begin();
        int random;
        try
        {
            random = std::rand() % (bunnies.size() - Bunny::vampCount - 1);
        }
        catch (...)
        {
            random = 0;
        }
        for (int j = 0; j <= random;)
        {
            ++it;
            if ((*it)->isVampire())
            {
                continue;
            }
            ++j;
        }
        std::cout << (*it)->getName() << " has been turned into a vampire " << std::endl;
        ++Bunny::vampCount;
        (*it)->turnVampire();
        turnedVamps++;
    }
    std::cout << born << " " << turnedVamps << std::endl;
    // convert healthy bunnies into vamps, //still broken
}

void BunnyManager::addBunny(const Bunny *mother) // create initial bunnies
{
    int random = std::rand() % 100;
    Gender sex = (random % 2 == 0) ? Gender::Male : Gender::Female;
    Colour colour = (mother == nullptr) ? Colour(random % Colour::Count) : mother->getColour();
    bool vampire = (random <= 2);
    std::string name;
    if (vampire)
    {
        random = std::rand() % vampireNames.size();
        name = vampireNames[random];
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
    std::cout << ((vampire) ? "Radioactive Mutant Vampire Bunny " : "Bunny ") << name << " was born!" << std::endl;
    bunnies.emplace_back(std::make_unique<Bunny>(sex, colour, name, 0, vampire));
}

void BunnyManager::printState()
{
    int healthy = bunnies.size() - Bunny::vampCount;
    std::cout << "Healthy bunnies: " << healthy << std::endl;
    std::cout << "Vampire bunnies: " << Bunny::vampCount << std::endl;
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
        printState();
        std::cout << "--------------------------" << std::endl;
        std::cout << std::endl;
        if (bunnies.size() >= 1000)
            cull();
        std::cout.flush();
        sleep(2);
        healthy = bunnies.size() - Bunny::vampCount;
    } while (healthy > 0);
    std::cout << std::endl
              << "There are no living bunnies " << std::endl;
    std::cout << "There are " << Bunny::vampCount << " Vampires " << std::endl;
}

void BunnyManager::cull()
{
    std::cout << "There is a cull" << std::endl;
    int amount = bunnies.size() / 2;
    std::list<std::shared_ptr<Bunny>>::iterator it;
    for (int i = 0; i < amount; ++i)
    {
        int rand = std::rand() % bunnies.size();
        it = bunnies.begin();
        std::advance(it, rand);
        // std::cout << (*it)->getName() << " has been culled " << std::endl;
        bunnies.erase(it);
    }
}

bool BunnyManager::oldMale()
{
    if (Bunny::maleCount > 0)
        return true;

    // for(auto &it : bunnies){
    //     if((*it).getAge() >= 2 && (*it).getSex() == 0)
    //         return true;
    // }
    // return false;
    return std::any_of(bunnies.begin(), bunnies.end(), [](const std::shared_ptr<Bunny> &it)
                       { return (*it).getAge() >= 2 && (*it).getSex() == 0; });
}