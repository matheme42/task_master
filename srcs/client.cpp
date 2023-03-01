#include "client.hpp"

void Client::disconnect() {
    fclose(stdin);
    listen = false;
}

int Client::checkArrowPress(int input) {
    if (input == 27) {
    input = getchar();
    if (input == 91) {
        input = getchar();
        if (input == 49) {
            getchar();getchar();getchar();
        }
        else if (input == 65) {
            if (history.size() > 0) {
                if (history_index > 0) {
                        termcaps.delLine();
                        line.clear();
                        line = history[--history_index];
                        termcaps.cursorPosition = line.size();
                        std::cout << line;
                    } else {
                        history_index = -1;
                        termcaps.delLine();
                        line.clear();
                    }
                }
            } else if (input == 66) {
            if (history.size() > 0) {
                history_index++;
                if (history_index >= history.size()) {
                    termcaps.delLine();
                    line.clear();
                    history_index = history.size();
                } else {
                    termcaps.delLine();
                    line.clear();
                    line = history[history_index];
                    termcaps.cursorPosition = line.size();
                    std::cout << line;
                }
            }
        } else if (input == 67) {
            //std::cout << "RIGHT" << std::endl;
        } else if (input == 68) {
            termcaps.moveLeft();
            //std::cout << "LEFT" << std::endl;
        } else if (input == 49) {
            termcaps.moveRight();
        }
        return (1);
    }
    } else if (input == 127) {
        if (line.size() > 0 && termcaps.cursorPosition > 0) {
            if (termcaps.cursorPosition != line.size()) {
                termcaps.del();
                std::cout << line.substr(termcaps.cursorPosition + 1, line.size() - termcaps.cursorPosition) << " ";
                line.erase(termcaps.cursorPosition, 1);
                int old_pos = termcaps.cursorPosition;
                termcaps.cursorPosition += line.size() + 1 - termcaps.cursorPosition;
                termcaps.moveLeft(line.size() - old_pos - 1);
            } else {
                line.pop_back();
                termcaps.del();
            }
                

        }
        //std::cout << "BACKSPACE" << std::endl;
        return (1);
    } else if (input == -1) {
        return (1);
    }
    return (0);
}

void Client::connect() {
    std::string command_ret;
    struct termios original_termios, new_termios;
    history_index = 0;
    // Sauvegarde les paramètres du terminal d'origine
    tcgetattr(STDIN_FILENO, &original_termios);
   // Configure le nouveau terminal pour l'entrée clavier non bloquante
    new_termios = original_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    std::cout << DEFAULT_PROMPT;
    listen = true;
    while (listen) {
        usleep(1000);
        int input = getchar();
        if (checkArrowPress(input)) continue;
        std::cout << (char)input;
        if (input != 10) {
            if (termcaps.cursorPosition != line.size() - 1) {
                line.replace(termcaps.cursorPosition, 1, (char*)&input);
            }
            else {
                line = line + (char)input;
            }
            termcaps.cursorPosition++;
        }
        else {
            if (onMessageReceive) command_ret = onMessageReceive(line.c_str());
            if (command_ret[0] != '\0') {
                std::cout << command_ret << std::endl;
            }
            history.push_back(line);
            history_index = history.size();
            termcaps.cursorPosition = 0;
            line.clear();
            if (listen) std::cout << DEFAULT_PROMPT;
        }
    }
    
    // Restaure les paramètres du terminal d'origine
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}
