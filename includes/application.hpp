#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <server.hpp>
#include <client.hpp>
#include <option.hpp>
#include <command.hpp>

#include <string>
#include <ostream>
#include <iostream>

class Application {
    private:
        /// modules
        Server server;
        Client client;
        Option preference;
        Command command;

        void setCommandCallback();
    public:
        void initWithArg(int ac, char **av);
        void start();
        void stop();
};

#endif