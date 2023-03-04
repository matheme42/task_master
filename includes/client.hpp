#ifndef CLIENT_HPP
#define CLIENT_HPP

#include<signal.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <functional>
#include <string>
#include <ostream>
#include <iostream>
#include <termios.h>
#include <termcaps.hpp>
#include <color.hpp>
#include <algorithm>

class Client
{
    #define DEFAULT_PROMPT BLUE << "taskmaster " << DARK_BLUE << "✗ " << DEFAULT_COLOR
    #define DEFAULT_PROMPT_SIZE 12

    std::vector <std::string> commandList = {
    "start", "stop", "restart", "status", "reload",
    "shutdown", "help", "exit", "quit"};

    private:
        Termcaps termcaps;
        std::string line;
        int cursor_position;
        int autocompletionStringIdx;
        std::string autocompletionString;


        std::vector <std::string> history;
        int history_index;

        int client_fd = 0;
        char buffer[2048] = { 0 };


        int manageControlKey(int input);
        int managekey(int input);
        void autocompletion();
        bool listen;
    public:
        std::function<std::string(const char *)> onMessageReceive;

        void start(int port);
        void start();
        void stop();
        void clear();
};




#endif