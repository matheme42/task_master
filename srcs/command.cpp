#include "command.hpp"

std::string Command::trim(const std::string &s)
{
    std::string tmp;
    size_t start = s.find_first_not_of(WHITESPACE);
    tmp = (start == std::string::npos) ? "" : s.substr(start);
    size_t end = tmp.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : tmp.substr(0, end + 1);
}

std::vector<std::string> Command::split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}


void Command::interrupt() {
    commandList.clear();
}

std::string Command::buildHelpString() {
    std::string help;

    help = help + DARK_BLUE + "system commands:\n" + DEFAULT_COLOR;
    help = help + GREEN + "  help        " + LIGHT_GREEN + "                         " + DEFAULT_COLOR + ": list all the command\n" + DEFAULT_COLOR;
    help = help + GREEN + "  reload      " + LIGHT_GREEN + "                         " + DEFAULT_COLOR + ": reload the config file\n" + DEFAULT_COLOR;
    help = help + GREEN + "  shutdown    " + LIGHT_GREEN + "                         " + DEFAULT_COLOR + ": shutdown taskmaster\n" + DEFAULT_COLOR;
    help = help + GREEN + "  quit | exit " + LIGHT_GREEN + "                         " + DEFAULT_COLOR + ": exit taskmaster\n" + DEFAULT_COLOR;
    if (enableBackgroundCommand) {
    help = help + GREEN + "  background  " + LIGHT_GREEN + " [port number]           " + DEFAULT_COLOR + ": turn taskmaster into sever listen on specified port\n" + DEFAULT_COLOR;
    }

    help = help + DARK_BLUE + "\nprocess commands:\n" + DEFAULT_COLOR;
    help = help + GREEN + "  status      " + LIGHT_GREEN + "                         " + DEFAULT_COLOR + ": list the status of all process\n" + DEFAULT_COLOR;
    help = help + GREEN + "  status      " + LIGHT_GREEN + " [process name] [etc...] " + DEFAULT_COLOR + ": give the status of the process\n" + DEFAULT_COLOR;
    help = help + GREEN + "  start       " + LIGHT_GREEN + " [process name] [etc...] " + DEFAULT_COLOR + ": start the given process\n" + DEFAULT_COLOR;
    help = help + GREEN + "  restart     " + LIGHT_GREEN + " [process name] [etc...] " + DEFAULT_COLOR + ": start the given process\n" + DEFAULT_COLOR;
    help = help + GREEN + "  stop        " + LIGHT_GREEN + " [process name] [etc...] " + DEFAULT_COLOR + ": stop the given process" + DEFAULT_COLOR;

    return help;
}


std::string Command::background(std::vector<std::string> v) {
    std::string command;
    std::string ret;
    std::string master_pass;
    std::string encrypt_key;
    int         port;


    command = v[0];
    char current_working_option = 0;
    encrypt_key = "";
    master_pass = "";
    port = 0;
    if (v.size() == 1) {
        ret = ret + DARK_BLUE + "taskmaster: " + LIGHT_RED + "command error: " + WHITE_BOLD + command + DEFAULT_COLOR + ": must be follow by a port number";
        return ret;
    }
    v.erase(v.begin());

    for (auto i : v) {
        if (current_working_option != 0) {
            if (current_working_option == 'k') encrypt_key = i;
            else if (current_working_option == 'P') master_pass = i;
            current_working_option = 0;
            continue;
        }
        if (i[0] == '-') {
            if (i.size() != 2 || (i[1] != 'k' && i[1] != 'P')) {
                ret = ret + + DARK_BLUE + "taskmaster: " + LIGHT_RED + "command error: " + WHITE_BOLD + command + DEFAULT_COLOR + ": invalide option: " + i[1];
                return ret;
            }
            current_working_option = i[1];
            continue;
        }

        int idx = 0;
        while (i[idx] && isdigit(i[idx++])) ;
        if (i[idx]) {
            ret = ret + DARK_BLUE + "taskmaster: " + LIGHT_RED + "command error: " + WHITE_BOLD + command + DEFAULT_COLOR + ": port must be an int";
            return ret;
        }
        port = atoi(i.c_str());
        if (port <= 0 || port > 65535) {
            ret = ret + DARK_BLUE + "taskmaster: " + LIGHT_RED + "command error: " + WHITE_BOLD + command + DEFAULT_COLOR + ": port must between 1 and 65535";
            return ret;
        }
    }

    if (port == 0) {
        ret = ret + DARK_BLUE + "taskmaster: " + LIGHT_RED + "command error: " + WHITE_BOLD + command + DEFAULT_COLOR + ": a port must be reffered";
        return ret;
    }

    if (onCommandBackground) {
        ret = ret + onCommandBackground(port, master_pass, encrypt_key);
        return ret;
    }
    return "\n";
}

std::string Command::interpreteCommand(std::string s) {
    std::string ret;

    
    std::vector<std::string> v = split(s, " ");
    std::transform(v[0].begin(), v[0].end(), v[0].begin(), [](unsigned char c){ return std::tolower(c); });

    if (v[0] == "shutdown") {
        if (onCommandShutdown) onCommandShutdown();
        return "exit";
    }
    if (v[0] == "quit" || v[0] == "exit") {
        if (onCommandExit) onCommandExit();
        return "exit";
    }
    if (v[0] == "help") {
        if (onCommandHelp) onCommandHelp();
        return buildHelpString();
    } else if (v[0] == "reload") {
        if (onCommandReload) ret = ret + onCommandReload();
        return ret;
    } else if (v[0] == "status") {
        if (v.size() == 1) {
               if (onCommandStatusAll) ret = ret + onCommandStatusAll();
               return ret;
        } else {
            for (auto it = begin(v) + 1; it != end(v); it++) {
               if (ret.size() != 0) ret = ret + "\n";
               if (onCommandStatus) ret = ret + onCommandStatus(*it);
            }
            return ret;
        }
    } else if (v[0] == "start" || v[0] == "restart" || v[0] == "stop") {
        if (v.size() == 1) {
            ret = ret + DARK_BLUE + "taskmaster: " + LIGHT_RED + "command error: " + WHITE_BOLD + v[0] + ": " + "must be follow by at least one process name" + DEFAULT_COLOR;
            return ret;
        } else {
            for (auto it = begin(v) + 1; it != end(v); it++) {
               if (ret.size() != 0) ret = ret + "\n";
               if (v[0] == "start" && onCommandStart) ret = ret + onCommandStart(*it);
               if (v[0] == "restart" && onCommandRestart) ret = ret + onCommandRestart(*it);
               if (v[0] == "stop" && onCommandStop) ret = ret + onCommandStop(*it);
            }
            return ret;
        }
    } else if (enableBackgroundCommand && v[0] == "background") {
        return background(v);
    }

    return "\n";
}

std::string Command::interprete(const char *s) {
    std::string commandLine = s;
    std::string commandsReturn;

    std::vector<std::string> v = split (s, "&&");
    for (auto i : v) {
        std::vector<std::string> w = split(i, ";");
        for (auto j : w) {
            commandList.push_back(trim(j));
        }
        w.clear();
    }
    v.clear();
    
    for (auto k : commandList) {
        std::string ret = interpreteCommand(k);
        if (commandsReturn.size() != 0) {
            commandsReturn = commandsReturn + "\n";
        }
        if (ret[0] == '\n') {
            commandsReturn = commandsReturn + DARK_BLUE + "taskmaster: " + LIGHT_RED + "command not found: " + WHITE_BOLD + k + DEFAULT_COLOR;
        } else {
            commandsReturn = commandsReturn + ret;
        }
    }
    commandList.clear();    
    return commandsReturn;
}