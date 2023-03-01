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

class Client
{
    #define DEFAULT_PROMPT "---(Taskmaster)-----------------\n"
    private:
        Termcaps termcaps;
        std::string line;

        std::vector <std::string> history;
        int history_index;

        int checkArrowPress(int input);
        bool listen;
    public:
        std::function<std::string(const char *)> onMessageReceive;

        void connect(int port);
        void connect();
        void disconnect();
};




#endif