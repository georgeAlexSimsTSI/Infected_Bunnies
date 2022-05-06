
#include <conio.h>
#include <unistd.h>

namespace userInput
{
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