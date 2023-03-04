#include "logging.hpp"

void Tintin_reporter::init(std::string outputfile) {
    this->outputfile = outputfile;
}

void Tintin_reporter::system(std::string message) {
    std::ofstream outfile;
    outfile.open(outputfile, std::ios_base::app);
    outfile << message << std::endl;
    outfile.close();
}

void Tintin_reporter::prompt(std::string message) {
    std::ofstream outfile;
    outfile.open(outputfile, std::ios_base::app);
    outfile << message << std::endl;
    outfile.close();
}

void Tintin_reporter::command(std::string message) {
    std::ofstream outfile;
    outfile.open(outputfile, std::ios_base::app);
    outfile << message << std::endl;
    outfile.close();
}

void Tintin_reporter::signal(std::string message) {
    std::ofstream outfile;
    outfile.open(outputfile, std::ios_base::app);
    outfile << message << std::endl;
    outfile.close();
}

void Tintin_reporter::error(std::string message) {
    std::ofstream outfile;
    outfile.open(outputfile, std::ios_base::app);
    outfile << message << std::endl;
    outfile.close();
}
