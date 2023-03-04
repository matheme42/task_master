#ifndef TINTIN_REPORTER_HPP
#define TINTIN_REPORTER_HPP

#include <string>
#include <ostream>
#include <iostream>
#include <fstream>

class Tintin_reporter {
    enum LEVEL {
        SYSTEM,
        PROMPT,
        COMMAND,
        SIGNAL,
        ERROR,
    };
    private:
        std::string outputfile = "/var/log/tintin/taskmaster.log";

    public:
        void init(std::string outputfile);
        void system(std::string message);
        void prompt(std::string message);
        void command(std::string message);
        void signal(std::string message);
        void error(std::string message);
};

#endif