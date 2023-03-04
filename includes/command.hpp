#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <color.hpp>

class Command {
    #define WHITESPACE " \n\r\t\f\v"
    /// \r --> retour en debut de ligne
    /// \t --> tabulation
    /// \n --> retour ligne
    /// \f --> retour ligne
    /// \v --> retour ligne


    private:
        std::vector<std::string>    commandList;

        /// function tools
        std::vector<std::string>    split(std::string s, std::string delimiter);
        std::string                 trim(const std::string &s);

        std::string                 interpreteCommand(std::string s);
        std::string                 buildHelpString();
    public:
        bool enableBackgroundCommand = false;

        void                        interrupt();
        std::string                 interprete(const char *s);

        /// les callbacks
        std::function<void()>        onCommandShutdown;
        std::function<void()>        onCommandExit;
        std::function<std::string()> onCommandReload;
        std::function<std::string(int port)> onCommandBackground;


        std::function<std::string(std::string)> onCommandStart;
        std::function<std::string(std::string)> onCommandRestart;
        std::function<std::string(std::string)> onCommandStop;
        std::function<std::string(std::string)> onCommandStatus;
        std::function<std::string()> onCommandStatusAll;
};

#endif