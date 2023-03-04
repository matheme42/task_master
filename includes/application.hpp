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
    enum Mode { SERVER, CLIENT};

    private:
        /// modules
        Server server;
        Client client;
        Option preference;
        Command command;
        Mode mode;

        void setCommandCallback();
    public:
        void initWithArg(int ac, char **av);
        void sigint();
        void start();
        void stop();
};

#endif