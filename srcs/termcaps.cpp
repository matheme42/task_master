#include "termcaps.hpp"

Termcaps::Termcaps() {
    if (setupterm(NULL, STDOUT_FILENO, NULL)) {
        fprintf(stderr, "unable to start TERMCAPS\n");
        return ;
    }

    /* Terminfo */
    column_count = tigetnum((char*)"cols");
    line_count = tigetnum((char*)"lines");
    cursorPosition = 0;
}

void Termcaps::del() {
   // if (cursorPosition % column_count == 0)
    //{
        // Séquence d'échappement de terminfo pour remonter le curseur d'un cran
     //   char *up_cursor = tparm(tigetstr((char*)"cuu1"));
        // Affichage de la séquence d'échappement pour remonter le curseur
     //   tputs(up_cursor, 1, putchar);
    //    return ;
    //}
    // obtenir les capacités du terminal
    char *cursor = tigetstr((char*)"cub1");

    // déplacer le curseur d'une position vers la gauche
    putp(cursor);
    std::cout << " ";
    putp(cursor);
    cursorPosition--;
}

void Termcaps::delLine() {
    char *clear_line = tigetstr((char*)"el1");
    const char* move_cursor = tigetstr((char*)"cr");
    
    tputs(clear_line, 1, putchar); // efface la ligne
    tputs(move_cursor, 1, putchar); // repositionne le curseur en début de ligne
    cursorPosition = 0;
}

void Termcaps::moveRight() {
    if (cursorPosition == column_count)
        return ;
    char *cursor = tigetstr((char*)"cuf1");
    putp(cursor);
    cursorPosition++;
}

void Termcaps::moveRight(int n) {
    if (n >= 0)
        moveRight(n - 1);
    moveRight();
}

void Termcaps::moveLeft() {
    if (cursorPosition == 0)
        return ;

    char *cursor = tigetstr((char*)"cub1");
    putp(cursor);
    cursorPosition--;
}


void Termcaps::moveLeft(int n) {
    if (n >= 0)
        moveLeft(n - 1);
    moveLeft();
}