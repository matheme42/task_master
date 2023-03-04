#ifndef CLIENT_HPP
#define CLIENT_HPP

// socket
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

// std
#include <vector>
#include <functional>
#include <string>
#include <string.h>
#include <algorithm>

// termcaps
#include <termios.h>
#include <termcaps.hpp>
#include <color.hpp>


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
        struct termios original_termios, new_termios;


        std::vector <std::string> history;
        int history_index;
        bool waitingForRemote;

        int client_fd = 0;
        char buffer[2048] = { 0 };


        int manageControlKey(int input);
        int managekey(int input);
        void autocompletion();
        void addLineTohistory();
        void newPrompt();
        void restoreKeyboard();
        void configureKeyboard();
        void sendCommandLineInLocal();
        void sendCommandLineInRemote();
        void readCommandLineInRemote();
        bool listen;
    public:
        std::function<std::string(const char *)> onMessageReceive;

        void start(int port);
        void start();
        void stop();
        void clear();
        void redraw();
};




#endif