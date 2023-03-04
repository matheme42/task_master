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
        /// mode
        Mode mode;


        /// modules
        Server server;
        Client client;
        Option preference;
        Command command;

        void setCommandCallback();
    public:

        /// initialize
        void initWithArg(int ac, char **av);

        /// start/stop
        void start();
        void stop();

        // reagir a signal
        void sigint();
        void sigwinch();
};

#endif