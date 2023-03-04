#include "client.hpp"

void Client::stop() {
    fclose(stdin);
    listen = false;
    fflush(stdout);
}

void Client::clear() {
    line.clear();
    cursor_position = 0;
    std::cout << '\n';
    std::cout << DEFAULT_PROMPT;
    fflush(stdout);
}

void Client::autocompletion() {
    int localAutocompletionStringIdx = 0;
   // if (line.size() == 0) return ;
    for (int i = 0; i < commandList.size(); i++) {
        if (commandList[i].find(line) == 0) {
            localAutocompletionStringIdx++;
            if (localAutocompletionStringIdx <= autocompletionStringIdx) {
                continue ;
            }
            autocompletionStringIdx = localAutocompletionStringIdx;
            auto autoComplete = commandList[i].substr(line.size());
            if (autocompletionString.size() > 0) {
                int del = autocompletionString.size();
                for (int i = 0; i < del; i++) {
                    termcaps.del();
                }
            }

            autocompletionString = autoComplete;
            std::cout << autoComplete;
            return ;
        }
    }
    if (localAutocompletionStringIdx != 0) {
        autocompletionStringIdx = 0;
        autocompletion();
    }
}

int Client::manageControlKey(int input) {
    if (input != 9 && autocompletionString.size() > 0) {
        line = line + autocompletionString;
        autocompletionStringIdx = 0;
        cursor_position += autocompletionString.size();
        autocompletionString.clear();
    }
    if (input == 27) { /// arrow key
    input = getchar();
    if (input == 91) {
        input = getchar();
        if (input == 49) {  /// ctrl + arrow
            getchar();getchar();getchar();
        }
        else if (input == 65) {
            if (history.size() > 0) {
                if (history_index > 0) {
                        termcaps.delLine();
                        line.clear();
                        line = history[--history_index];
                        cursor_position = line.size();
                        std::cout << DEFAULT_PROMPT << line;
                    } else {
                        history_index = -1;
                        termcaps.delLine();
                        line.clear();
                        cursor_position = 0;
                        std::cout << DEFAULT_PROMPT;
                    }
                }
            } else if (input == 66) {
            if (history.size() > 0) {
                history_index++;
                if (history_index >= history.size()) {
                    termcaps.delLine();
                    line.clear();
                    history_index = history.size();
                    cursor_position = 0;
                    std::cout << DEFAULT_PROMPT;
                } else {
                    termcaps.delLine();
                    line.clear();
                    line = history[history_index];
                    cursor_position = line.size();
                    std::cout << DEFAULT_PROMPT << line;
                }
            }
        } else if (input == 67 && cursor_position < line.size()) {
            cursor_position += 1;
            if ((cursor_position + DEFAULT_PROMPT_SIZE + 1) % termcaps.column_count == 0) {
                termcaps.moveDown();
                return (1);
            }
            termcaps.moveRight();
        } else if (input == 68 && cursor_position > 0) {
            if ((cursor_position + DEFAULT_PROMPT_SIZE + 1) % termcaps.column_count == 0) {
                cursor_position -= 1;
                termcaps.moveUp();
                termcaps.moveRight(termcaps.column_count);
                return (1);
            }
            cursor_position -= 1;
            termcaps.moveLeft();
        }
        return (1);
    }
    } else if (input == 127) { /// backspace
        if (cursor_position > 0) {
            if ((cursor_position + DEFAULT_PROMPT_SIZE + 1) % termcaps.column_count == 0) {
                termcaps.delUp();
            } else {
                termcaps.del();
            }
            cursor_position -= 1;
            line.erase(cursor_position, 1);
            termcaps.saveCursorPosition();
            std::cout << line.substr(cursor_position, line.size() - cursor_position);
            std::cout << ' ';
            termcaps.restoreCursorPosition();
        }
        return (1);
    } else if (input == -1) {
        return (1);
    } else if (input == 9) { /// tabulation
        autocompletion();
        return (1);
    } else if (input == 12) { /// clear
        termcaps.clear();
        line.clear();
        cursor_position = 0;
        std::cout << DEFAULT_PROMPT;
        return (1);
    }
    return (0);
}

int Client::managekey(int input) {
    std::cout << (char)input;
    if (input == '\n') return (1);
    cursor_position += 1;
    if (cursor_position > line.size())
        line.push_back((char)input);
    else {
        termcaps.saveCursorPosition();
        line.insert(cursor_position - 1, 1, (char)input);
        std::cout << line.substr(cursor_position, line.size() - cursor_position);
        termcaps.restoreCursorPosition();
    }
    if ((cursor_position + DEFAULT_PROMPT_SIZE + 1) % termcaps.column_count == 0) {
        termcaps.moveDown();
    }
    return (0);
}

void Client::start() {
    std::string command_ret;
    struct termios original_termios, new_termios;
    int input;

    history_index = 0;
    cursor_position = 0;
    autocompletionStringIdx = 0;
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
        if ((input = getchar()) == 4) {
            if (line.size() == 0) {
                std::cout << '\r' << DEFAULT_PROMPT << '\n';
                stop();
            } 
        }
        else if (manageControlKey(input)) continue;
        else if (managekey(input)) {
            if (client_fd == 0) {
            if (onMessageReceive && line.size() > 0) {
                command_ret = onMessageReceive(line.c_str());
                if (command_ret[0] != '\0') std::cout << command_ret << std::endl;
            }
            } else {
                send(client_fd, line.c_str(), line.size() + 1, 0);
                int valread = read(client_fd, buffer, 2048);
                if (!strcmp(buffer, "shutdown")) {
                    stop();
                } else {
                    if (buffer[0] != '\0') std::cout << buffer << std::endl;
                }
                bzero(buffer, valread);
            }

            if (line.size() > 0) {
            auto it = std::find(history.begin(), history.end(), line);
            if (it != history.end())
                history.erase(it);
            history.push_back(line);
            history_index = history.size();
            }
            line.clear();
            cursor_position = 0;
            if (listen) std::cout << DEFAULT_PROMPT;
        }
    }
    
    // Restaure les paramètres du terminal d'origine
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

       #include <arpa/inet.h>
       #include <sys/socket.h>

void Client::start(int port) {
    bzero(buffer, sizeof(buffer));

    int status, valread;
    struct sockaddr_in serv_addr;
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return ;
    }
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
  
    // Convert IPv4 and IPv6 addresses from text to binary
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return ;
    }
  
    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))
        < 0) {
        printf("Connection Failed \n");
        return ;
    }
   // send(client_fd, hello, strlen(hello), 0);
   // valread = read(client_fd, buffer, 1024);
   // printf("%s %d\n", buffer, valread);

    start();
  
    // closing the connected socket
    close(client_fd);
}

void Client::redraw() {
    termcaps.clear();
    std::cout << DEFAULT_PROMPT << line;
    if (autocompletionString.size() > 0) {
        std::cout << autocompletionString;
    }
    fflush(stdout);
}