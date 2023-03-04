#ifndef TERMCAPS
#define TERMCAPS

#include <iostream>
#include <term.h>
#include <unistd.h>
#include <sys/ioctl.h>


class Termcaps
{
    public:
        int column_count;
        int line_count;

        Termcaps();

        void getTermSize();

        void del();
        void delUp();
        void delLine();
        void clear();

        void moveLeft();
        void moveLeft(int n);
        void moveRight();
        void moveRight(int n);
        void moveUp();
        void moveDown();

        void saveCursorPosition();
        void restoreCursorPosition();
};

#endif