#include "../include/bunnyManager.h"

#include <iostream>
#include <fstream>

inline void loadData(std::vector<std::string> &names, const std::string &fileName)
{
    std::ifstream namesFile;
    std::string name;
    try
    {
        namesFile.open(fileName);
        while (namesFile)
        {
            std::getline(namesFile, name);
            if (!name.empty())
                names.push_back(name);
        }
        namesFile.close();
    }
    catch (...)
    {
        namesFile.close();
    }
}

int main()
{
    system("cls");

    // male names are from https://www.verywellfamily.com/top-1000-baby-boy-names-2757618
    std::vector<std::string> maleBunnyNames;
    loadData(maleBunnyNames, "../maleNames.txt");
    if (maleBunnyNames.size() == 0)
        maleBunnyNames = {"Jason", "Liam", "Noah", "Oliver", "Peter"};

    // female names are from https://www.verywellfamily.com/top-1000-baby-girl-names-2757832
    std::vector<std::string> femaleBunnyNames;
    loadData(femaleBunnyNames, "../femaleNames.txt");
    if (femaleBunnyNames.size() == 0)
        femaleBunnyNames = {"Olivia", "Emma", "Charlotte", "Amelia", "Ava"};

    std::vector<std::string> infectedBunnyNames{"Patient Zero"};

    int gridHeight = 20, gridWidth = 20, startingNumber = 20; // would recommend a smaller grid size as it is easier to read
    bool VERBOSE = false;
    BunnyManager manager = BunnyManager(maleBunnyNames, femaleBunnyNames, infectedBunnyNames, gridHeight, gridWidth, startingNumber, VERBOSE); // numbers are x , y and starting number
    manager.run();
}