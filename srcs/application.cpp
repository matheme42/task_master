#include "application.hpp"


void Application::start() {
    std::ofstream outfile;

    if (preference.log_path.size()) {
        outfile.open(preference.log_path, std::ios_base::app);
    }
    
    client.onMessageReceive = ([&] (const char *message) {
        if (outfile.is_open()) outfile << message << std::endl; 
        return command.interprete(message);
    });

    server.onMessageReceive = ([&] (const char *message) {
        if (outfile.is_open()) outfile << message << std::endl; 
        return command.interprete(message);
    });
    setCommandCallback();

    if (preference.port) {
        if (preference.config_path.size() != 0) {
            server.start(preference.port);
        } else {
            client.start(preference.port);
        }
    } else {
        client.start();
    }

    if (preference.log_path.size()) {
        outfile.close();
    }
}

void Application::stop() {
    preference.port && preference.config_path.size() ? server.stop() : client.stop();
    command.interrupt();
}

void Application::initWithArg(int ac, char **av) {
    preference.configure(ac, av);
    mode = preference.port && preference.config_path.size() != 0 ? SERVER : CLIENT;
}

void Application::sigint() {
    if (mode == SERVER) {
        server.stop();
    } else {
        client.clear();
    }
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
        return "shutdown";
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
