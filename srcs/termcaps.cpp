#include "termcaps.hpp"

Termcaps::Termcaps() {
    if (setupterm(NULL, STDOUT_FILENO, NULL)) {
        fprintf(stderr, "unable to start TERMCAPS\n");
        return ;
    }
    column_count = tigetnum("cols");
    line_count = tigetnum("lines");
}

void Termcaps::del() {
    putp(tigetstr((char*)"cub1"));
    std::cout << " ";
    putp(tigetstr((char*)"cub1"));
}

void Termcaps::delUp() {
    moveUp();
    moveRight(column_count);
    std::cout << " ";
}

void Termcaps::delLine() {
    tputs(tigetstr((char*)"el1"), 1, putchar); // efface la ligne
    tputs(tigetstr((char*)"el"), 1, putchar); // efface la ligne
    tputs(tigetstr((char*)"cr"), 1, putchar); // repositionne le curseur en début de ligne
}

void Termcaps::saveCursorPosition() {
    tputs(tigetstr((char*)"sc"), 1, putchar);
}

void Termcaps::restoreCursorPosition() {
    tputs(tigetstr((char*)"rc"), 1, putchar);
}

void Termcaps::clear() {
    tputs(tigetstr((char*)"clear"), 1, putchar);
    column_count = tigetnum("cols");
    line_count = tigetnum("lines");
}

void Termcaps::moveRight() {
    char *cursor = tigetstr((char*)"cuf1");
    putp(cursor);
}

void Termcaps::moveRight(int n) {
    if (n >= 0)
        moveRight(n - 1);
    moveRight();
}

void Termcaps::moveLeft() {
    char *cursor = tigetstr((char*)"cub1");
    putp(cursor);
}


void Termcaps::moveLeft(int n) {
    if (n >= 0)
        moveLeft(n - 1);
    moveLeft();
}

void Termcaps::moveUp() {
    char *cursor = tigetstr((char*)"cuu1");
    putp(cursor);
}

void Termcaps::moveDown() {
    char *cursor = tigetstr((char*)"cud1");
    putp(cursor);
}