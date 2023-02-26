#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <ostream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

class Command {
    #define WHITESPACE " \n\r\t\f\v"

    private:
        std::vector<std::string>    commandList;

        /// function tools
        std::vector<std::string>    split(std::string s, std::string delimiter);
        std::string                 trim(const std::string &s);

        std::string                 interpreteCommand(std::string s);
        std::string                 buildHelpString();
    public:
        void                        interrupt();
        std::string                 interprete(const char *s);

        /// les callbacks
        std::function<void()>        onCommandShutdown;
        std::function<std::string()> onCommandReload;

        std::function<std::string(std::string)> onCommandStart;
        std::function<std::string(std::string)> onCommandRestart;
        std::function<std::string(std::string)> onCommandStop;

        std::function<std::string(std::string)> onCommandStatus;
        std::function<std::string()> onCommandStatusAll;
};

#endif