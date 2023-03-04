#include "application.hpp"


int  Application::create_directory_recursive(char *dir, mode_t mode)
{
    if (!dir) {
        errno = EINVAL;
        return 1;
    }

    if (strlen(dir) == 1 && dir[0] == '/') return 0;
    create_directory_recursive(dirname(strdupa(dir)), mode);
    int ret = mkdir(dir, mode);
    if (errno == EEXIST) {
        return 0;
    }
    return ret;
}


bool Application::CheckForInstance() {
   
    if (FILE *file = fopen(LOCKFILE, "r+")) {
        fclose(file);
        std::string ret = "an instance of taskmaster is already running";
        std::cout << LIGHT_RED << ret << DEFAULT_COLOR << std::endl;
        reporter.error(ret);
        return true;
    }

    std::string path = LOCKFILE;
    std::string directory = path.substr(0, path.find_last_of('/'));
    if (create_directory_recursive((char*)directory.c_str(), S_IRWXU))
        std::cout << "can't log to file:" << path;

    std::ofstream outfile (LOCKFILE);
    outfile.close();
    return false;
}

void Application::start() {
    if (mode == NONE) {
        reporter.system("system start (local)");
        command.enableBackgroundCommand = true;
        if (CheckForInstance()) return ;
        client.start();
        if (restart) start();
        remove(LOCKFILE);
        return ;
    }

    if (mode == SERVER) {
        reporter.system("system start (server)");
        command.enableBackgroundCommand = false;
        if (!restart) {
            if (CheckForInstance()) return ;
            std::string ret = server.configure(preference.port);
            if (ret.size()) {
                std::cout << ret << std::endl;
                return ;
            }
        }
        server.start();
        remove(LOCKFILE);
        return ;
    }
    client.start(preference.port);
}

void Application::configureLogger() {
    if (preference.log_path.size() == 0) {
        std::cout << DARK_BLUE << "log path not set. using default path: " << WHITE_BOLD << LOGGING_DEFAULT_PATH << std::endl;

        std::string path = LOGGING_DEFAULT_PATH;
        std::string directory = path.substr(0, path.find_last_of('/'));
        if (create_directory_recursive((char*)directory.c_str(), S_IRWXU))
            std::cout << "can't log to file:" << path;

        reporter.init(LOGGING_DEFAULT_PATH);
    } else {

        std::string path = preference.log_path;
        std::string directory = path.substr(0, path.find_last_of('/'));
        if (create_directory_recursive((char*)directory.c_str(), S_IRWXU))
            std::cout << "can't log to file:" << path;

        reporter.init(preference.log_path);
    }
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

    if (mode == CLIENT) return ;

    configureLogger();
    setCommandCallback();
    client.onMessageReceive = ([&] (const char *message) {
        return command.interprete(message);
    });
    server.onMessageReceive = ([&] (const char *message) {
        return command.interprete(message);
    });
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
