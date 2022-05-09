
#include <conio.h>
#include <unistd.h>

namespace userInput
{
    /**
     * @brief A program that will wait a given amount of seconds for any keyboard input
     *
     * @param seconds number of seconds to wait
     * @return char the entered character
     */
    char waitForCharInput(int seconds)
    {
        char c = '_';
        while (seconds != 0)
        {
            if (_kbhit())
            {
                c = _getch();
                break;
            }
            sleep(1);
            --seconds;
        }
        return c;
    }
}