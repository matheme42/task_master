#include "application.hpp"


void Application::start() {
    std::ofstream outfile;
    if (!restart) {
        if (preference.log_path.size()) outfile.open(preference.log_path, std::ios_base::app);
        setCommandCallback();
        client.onMessageReceive = ([&] (const char *message) {
            if (outfile.is_open()) outfile << message << std::endl; 
            return command.interprete(message);
        });
        server.onMessageReceive = ([&] (const char *message) {
            if (outfile.is_open()) outfile << message << std::endl; 
            return command.interprete(message);
        });
    }

    if (mode == NONE) {
        command.enableBackgroundCommand = true;
        client.start();
        if (restart) start();
    }
    else if (mode == SERVER) {
        command.enableBackgroundCommand = false;
        if (!restart) {
            std::string ret = server.configure(preference.port);
            if (ret.size()) {
                std::cout << ret << std::endl;
                return ;
            }
        }
        restart = false;
        server.start();
        return ;
    } else {
        client.start(preference.port);
    }
    if (preference.log_path.size()) outfile.close();
}

void Application::stop() {
    if (mode == SERVER) server.stop();
    else client.stop();
    command.interrupt();
}

void Application::initWithArg(int ac, char **av) {
    restart = false;
    preference.configure(ac, av);
    if (preference.port == 0) mode = NONE;
    else if (preference.config_path.size() == 0) mode = CLIENT;
    else mode = SERVER;
}

void Application::sigint() {
    if (mode == SERVER) server.stop();
    else client.clear();
}

void Application::sigwinch() {
    client.redraw();
}

void Application::setCommandCallback() {
    command.onCommandShutdown = ([&](){stop();});

    command.onCommandReload = ([&](){
        std::string ret;
        return ret;
    });

    command.onCommandBackground = ([&](int port){
        std::string ret = server.configure(port);
        if (ret.size()) return ret;
        client.stop();
        mode = SERVER;
        preference.port = port;
        restart = true;
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
