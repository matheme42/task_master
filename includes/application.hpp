#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <prompt.hpp>
#include <option.hpp>
#include <command.hpp>

#include <string>
#include <ostream>
#include <iostream>

class Application {
    private:
        /// modules
        Prompt prompt;
        Option preference;
        Command command;

        void setCommandCallback();
    public:
        void initWithArg(int ac, char **av);
        void start();
        void stop();
};

#endif