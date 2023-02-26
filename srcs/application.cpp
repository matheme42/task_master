#include "application.hpp"


void Application::start() {
    std::ofstream outfile;

    if (preference.log_path.size()) {
        outfile.open(preference.log_path, std::ios_base::app);
    }
    prompt.onMessageReceive = ([&] (const char *message) {
        if (outfile.is_open()) outfile << message << std::endl; 
        return command.interprete(message);
    });
    setCommandCallback();

    preference.port ? prompt.start(preference.port) : prompt.start();
    if (preference.log_path.size()) {
        outfile.close();
    }
}

void Application::stop() {
    prompt.stop();
    command.interrupt();
}

void Application::initWithArg(int ac, char **av) {
    preference.configure(ac, av);
}

void Application::setCommandCallback() {
    command.onCommandShutdown = ([&](){stop();});

    command.onCommandReload = ([&](){
        std::string ret;
        return ret;
    });

    command.onCommandStart = ([&](std::string process){
        std::string ret;
        return ret;
    });

    command.onCommandStop = ([&](std::string process){
        std::string ret;
        return ret;
    });

    command.onCommandRestart = ([&](std::string process){
        std::string ret;
        return ret;
    });

    command.onCommandStatus = ([&](std::string process){
        std::string ret;
        return ret;
    });

    command.onCommandStatusAll = ([&](){
        std::string ret;
        return ret;
    });
}
