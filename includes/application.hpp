#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <prompt.hpp>
#include <option.hpp>

#include <string>
#include <ostream>
#include <iostream>

class Application {
    private:
        /// modules
        Prompt prompt;
        Option preference;

    public:
        void initWithArg(int ac, char **av);
        void start();
};


#endif