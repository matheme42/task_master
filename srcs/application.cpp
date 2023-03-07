#include "application.hpp"


int  Application::create_directory_recursive(char *dir, mode_t mode)
{
    if (!dir) {
        errno = EINVAL;
        return 1;
    }

    if (strlen(dir) == 1 && dir[0] == '/') return 0;
    create_directory_recursive(dirname(dir), mode);
    int ret = mkdir(dir, mode);
    if (errno == EEXIST) {
        return 0;
    }
    return ret;
}


bool Application::CheckForInstance() {

    DIR* dir = opendir("/proc");
    if (dir == nullptr) {
        std::cerr << LIGHT_RED << "Failed to open /proc directory\n" << DEFAULT_COLOR;
        return (false);
    }
    pid_t pid = getpid();

    dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        // Check if the entry is a directory and its name is a number
        if (entry->d_type == DT_DIR && std::isdigit(entry->d_name[0])) {
            std::string cmdline_file = std::string("/proc/") + entry->d_name + "/status";
            std::ifstream ifs(cmdline_file);
            std::string cmdline;
            std::getline(ifs, cmdline);
            cmdline = cmdline.substr(cmdline.find("\t") + 1);

            // Check if the cmdline contains the process name
            if (cmdline.find("taskmaster") != std::string::npos && std::stoi(entry->d_name) != pid) {
                std::string ret = "An instance of taskmaster is already running pid: " + std::string(entry->d_name);
                std::cout << LIGHT_RED << ret << DEFAULT_COLOR << std::endl;
                reporter.error(ret);
                closedir(dir);
                return (true);
            }
        }
    }
    closedir(dir);
    return (false);
}


bool Application::CheckForLockFile() {
   if (CheckForInstance()) return true;
    if (FILE *file = fopen(LOCKFILE, "r+")) {
        fclose(file);
        std::string ret = "taskmaster don't quit proprely, a lock file prevents starting: " + std::string(LOCKFILE) + " please remove it and try again";
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
        reporter.system("start (local)");
        command.enableBackgroundCommand = true;
        if (CheckForLockFile()) {
            reporter.system("stop");
            reporter.close();
            return ;
        }
        client.start();
        if (restart) {
            reporter.system("switch to server mode");
            start();
            return ;
        } 
        remove(LOCKFILE);
        reporter.system("stop");
        reporter.close();
        return ;
    }

    if (mode == SERVER) {
        reporter.system("start (server)");
        command.enableBackgroundCommand = false;
        if (!restart) {
            if (CheckForLockFile()) {
                reporter.system("stop");
                reporter.close();
                return ;
            }
            std::string ret = server.configure(preference.port, preference.master_password);
            if (ret.size()) {
                std::cout << LIGHT_RED << ret << std::endl;
                remove(LOCKFILE);
                reporter.system("stop");
                reporter.close();
                return ;
            }
        }
        server.start();
        remove(LOCKFILE);
        reporter.system("stop");
        reporter.close();
        return ;
    }

    client.start(preference.ip, preference.port);
}

void Application::configureLogger() {
    if (preference.log_path.size() == 0) {
        std::cout << DARK_BLUE << "log path not set. using default path: " << WHITE_BOLD << LOGGING_DEFAULT_PATH << std::endl;

        std::string path = LOGGING_DEFAULT_PATH;
        if (path.find('/') != std::string::npos) {
            std::string directory = path.substr(0, path.find_last_of('/'));
            if (create_directory_recursive((char*)directory.c_str(), S_IRWXU))
                std::cout << "can't log to file:" << path;
        }

        reporter.init(LOGGING_DEFAULT_PATH);
    } else {

        std::string path = preference.log_path;
        std::string directory = path.substr(0, path.find_last_of('/'));
        if (path.find('/') != std::string::npos) {
            if (create_directory_recursive((char*)directory.c_str(), S_IRWXU))
                std::cout << "can't log to file:" << path;
        }

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

    client.encrypter = ([&] (std::string message) {
        std::string encryptedMessage;
        encryptedMessage = cryptage.crypter(message, preference.crytage_key);
        return encryptedMessage;
    });

    client.decrypter = ([&] (std::string message) {
        std::string decryptedMessage;
        decryptedMessage = cryptage.decrypter(message, preference.crytage_key);
        return decryptedMessage;
    });

    if (mode == CLIENT) return ;

    configureLogger();

    setCommandCallback();
    setServerCallback();

    client.onMessageReceive = ([&] (const char *message) {
        reporter.prompt(message);
        return command.interprete(message);
    });


}

void Application::sigint() {
    reporter.signal("sigint");
    if (mode == SERVER) server.stop();
    else client.clear();
}

void Application::sigwinch() {
    client.redraw();
}

void Application::setServerCallback() {
    server.decrypter = ([&] (std::string message) {
        std::string decryptedMessage;
        decryptedMessage = cryptage.decrypter(message, preference.crytage_key);
        return decryptedMessage;
    });

    server.encrypter = ([&] (std::string message) {
        std::string encryptedMessage;
        encryptedMessage = cryptage.crypter(message, preference.crytage_key);
        return encryptedMessage;
    });

    server.onMessageReceive = ([&] (const char *message) {
        reporter.prompt(message);
        return command.interprete(message);
    });
}

void Application::setCommandCallback() {
    command.onCommandShutdown = ([&](){
        reporter.command("shutdown");
        stop();
        });

    command.onCommandReload = ([&](){
        reporter.command("reload");
        std::string ret;
        return ret;
    });

    command.onCommandBackground = ([&](int port){
        reporter.command("background port: " + std::to_string(port));
        std::string ret = server.configure(port, "");
        if (ret.size()) {
            ret = LIGHT_RED + ret + DEFAULT_COLOR;
            return ret;
        } 
        client.stop();
        mode = SERVER;
        preference.port = port;
        restart = true;
        return ret;
    });

    command.onCommandStart = ([&](std::string process){
        reporter.command("start process: " + process);
        std::string ret;
        return ret;
    });

    command.onCommandStop = ([&](std::string process){
        reporter.command("stop process: " + process);
        std::string ret;
        return ret;
    });

    command.onCommandRestart = ([&](std::string process){
        reporter.command("restart process: " + process);
        std::string ret;
        return ret;
    });

    command.onCommandStatus = ([&](std::string process){
        reporter.command("status process: " + process);
        std::string ret;
        return ret;
    });

    command.onCommandStatusAll = ([&](){
        reporter.command("status all");
        std::string ret;
        return ret;
    });

    command.onCommandHelp = ([&](){
        reporter.command("help");
    });
}
