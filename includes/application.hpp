#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <server.hpp>
#include <client.hpp>
#include <option.hpp>
#include <command.hpp>
#include <logging.hpp>
#include <cryptage.hpp>

#include <libgen.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <string>

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
        Cryptage cryptage;

        /// a flag to tell the application need to restart
        bool restart;

        int  create_directory_recursive(char *dir, mode_t mode);
        void setCommandCallback();
        void setServerCallback();
        void configureLogger();
        bool CheckForLockFile();
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
