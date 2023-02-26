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

    help = help + "commands:\n";
    help = help + "  help                             : list all the command\n";
    help = help + "  reload                           : reload the config file\n";
    help = help + "  status                           : list the status of all process\n";
    help = help + "  status   [process name] [etc...] : give the status of the process\n";
    help = help + "  start    [process name] [etc...] : start the given process\n";
    help = help + "  restart  [process name] [etc...] : start the given process\n";
    help = help + "  stop     [process name] [etc...] : stop the given process\n";
    help = help + "  shutdown                         : shutdown taskmaster";
    return help;
}

std::string Command::interpreteCommand(std::string s) {
    std::string ret;

    
    std::vector<std::string> v = split(s, " ");
    std::transform(v[0].begin(), v[0].end(), v[0].begin(), [](unsigned char c){ return std::tolower(c); });

    if (v[0] == "shutdown") {
        if (onCommandShutdown) onCommandShutdown();
        return ret;
    }
    if (v[0] == "help") {
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
            return "command error >> " + v[0] + " must be follow by at least one process name";
        } else {
            for (auto it = begin(v) + 1; it != end(v); it++) {
               if (ret.size() != 0) ret = ret + "\n";
               if (v[0] == "start" && onCommandStart) ret = ret + onCommandStart(*it);
               if (v[0] == "restart" && onCommandRestart) ret = ret + onCommandRestart(*it);
               if (v[0] == "stop" && onCommandStop) ret = ret + onCommandStop(*it);
            }
            return ret;
        }
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
            commandsReturn = commandsReturn + "command not found >> " + k + " (try command: help)";
        } else {
            commandsReturn = commandsReturn + ret;
        }
    }
    commandList.clear();    
    return commandsReturn;
}