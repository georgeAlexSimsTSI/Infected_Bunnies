
#include <conio.h>
#include <unistd.h>

/**
 * @brief namespace for userinput functions
 */
namespace userInput
{
    /**
     * @brief method that waits a certain amount of time
     * @param seconds amount of time to wait
     * @return char representing the user input
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