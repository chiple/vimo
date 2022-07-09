#include <curses.h>

int main()
{
    int key;

    initscr(); cbreak(); noecho();

    while (1)
    {

        key = getch();
        printw ("%u\n", key);
    }

    return 0;
}
