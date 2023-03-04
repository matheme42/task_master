#include "logging.hpp"

void Tintin_reporter::init(std::string outputfile) {
    this->outputfile = outputfile;
}

std::string Tintin_reporter::getTime() {
    // obtenir le temps actuel
    auto now = std::chrono::system_clock::now();

    // convertir en temps local
    auto time = std::chrono::system_clock::to_time_t(now);
    auto local_time = std::localtime(&time);

   // formater la date et l'heure dans le format [DD/MM/YYYY-HH:MM:SS]
    std::stringstream ss;
    ss << std::setfill('0')
       << std::setw(2) << local_time->tm_mday << "/"
       << std::setw(2) << local_time->tm_mon + 1 << "/"
       << std::setw(4) << local_time->tm_year + 1900 << "-"
       << std::setw(2) << local_time->tm_hour << ":"
       << std::setw(2) << local_time->tm_min << ":"
       << std::setw(2) << local_time->tm_sec;

    // stocker la date et l'heure formatées dans une chaîne de caractères
    std::string formatted_time = ss.str();

    return formatted_time;
}

void Tintin_reporter::log(std::string message, std::string color, std::string level) {
    std::ofstream outfile;
    outfile.open(outputfile, std::ios_base::app);
    outfile << WHITE_BOLD << "[" << getTime() << "] " << color << level + " " << DEFAULT_COLOR << message << std::endl;
    outfile.close();
}

void Tintin_reporter::system(std::string message) {
    log(message, DARK_BLUE, "[system] ");
}

void Tintin_reporter::prompt(std::string message) {
    log(message, DEFAULT_COLOR, "[prompt] ");
}

void Tintin_reporter::command(std::string message) {
    log(message, GREEN, "[command]");
}

void Tintin_reporter::signal(std::string message) {
    log(message, PINK, "[signal] ");
}

void Tintin_reporter::error(std::string message) {
    log(message, DARK_RED, "[error]  ");
}
