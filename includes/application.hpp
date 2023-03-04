#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <server.hpp>
#include <client.hpp>
#include <option.hpp>
#include <command.hpp>
#include <logging.hpp>

class Application {
    enum Mode { SERVER, CLIENT, NONE};

    private:
        /// mode
        Mode mode;

        /// logging
        Tintin_reporter reporter;

        /// modules
        Server server;
        Client client;
        Option preference;
        Command command;

        /// a flag to tell the application need to restart
        bool restart;

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