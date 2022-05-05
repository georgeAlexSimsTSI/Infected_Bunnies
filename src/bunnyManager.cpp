#include "../include/bunnyManager.h"

void BunnyManager::increment()
{
    Bunny::vampCount = 0;
    Bunny::maleCount = 0; // only counts eligible bunnies
    Bunny::femaleCount = 0;

    // due to nature of list oldest naturally come to the front excluding vampires
    std::list<std::unique_ptr<Bunny>>::iterator it = bunnies.begin();
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
        }
    }

    // create new bunnies
    if (Bunny::maleCount > 0)
    {
        for (unsigned int i = 0; i < Bunny::femaleCount; ++i)
        {
            addBunny();
        }
    }

    // convert healthy bunnies into vamps

    // unsigned int healthyRabits = bunnies.size() - Bunny::vampCount;
    for (int i = 0; i < Bunny::vampCount; ++i)
    { // this isn't great, redo it later
        do
        {
            it = bunnies.begin();
            // std::advance(it,(rand() % bunnies.size()));
            // if((*it)->isVampire())
            //     continue;
            // (*it)->turnVampire();
            // break;
            for (int j = 0; j < bunnies.size(); ++i)
            {
                if ((*it)->isVampire()){
                    ++it;
                    continue;
                }
                (*it)->turnVampire();
            }

        } while ((bunnies.size() - Bunny::vampCount - i) > 0);
    }
}

void BunnyManager::addBunny()
{
    int random = std::rand() % 100;
    Gender sex = (random % 2 == 0) ? Gender::Male : Gender::Female;
    Colour colour = Colour(random % Colour::Count);
    bool vampire = (random <= 2);
    std::string name;

    if (vampire)
    {
        random = std::rand() % vampireNames.size();
        name = vampireNames[random];
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
