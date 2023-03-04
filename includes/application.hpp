#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <server.hpp>
#include <client.hpp>
#include <option.hpp>
#include <command.hpp>
#include <logging.hpp>

#include <libgen.h>
#include <sys/stat.h>


class Application {
    enum Mode { SERVER, CLIENT, NONE};
    #define LOCKFILE "/var/lock/taskmaster.lock"

    private:
        /// mode
        Mode mode;

        /// modules
        Server server;
        Client client;
        Option preference;
        Command command;

        /// a flag to tell the application need to restart
        bool restart;

        int  create_directory_recursive(char *dir, mode_t mode);
        void setCommandCallback();
        void configureLogger();
        bool CheckForInstance();
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