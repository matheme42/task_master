#ifndef TERMCAPS
#define TERMCAPS

#include <iostream>
#include <term.h>
#include <unistd.h>


class Termcaps
{
    private:
    int column_count;
    int line_count;

        
    public:
        int cursorPosition;
        Termcaps();
        void del();
        void delLine();

        void moveLeft();
        void moveLeft(int n);
        void moveRight();
        void moveRight(int n);
};

#endif