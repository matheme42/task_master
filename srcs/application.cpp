#include "application.hpp"

void Application::start() {
    std::ofstream outfile;

    if (preference.log_path.size()) {
        outfile.open(preference.log_path, std::ios_base::app);
    }
    prompt.onMessageReceive = ([&] (const char *message) {
        if (outfile.is_open()) outfile << message << std::endl; 
    });
    preference.port ? prompt.start(preference.port) : prompt.start();
    outfile.close();
}

void Application::stop() {
    prompt.stop();
}

void Application::initWithArg(int ac, char **av) {
    preference.configure(ac, av);
}
