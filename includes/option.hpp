#ifndef OPTION_HPP
#define OPTION_HPP

#include <map>
#include <cstring>

#include <ostream>
#include <iostream>
#include <fstream>

#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>

#include <color.hpp>

class Option {
    private:
        /// enums
        enum class type_t { number, none, letter, sentence, infile, outfile};
        /// constante
        std::map<char, type_t> state = {
            {'p', type_t::number},
            {'c', type_t::infile},
            {'l', type_t::outfile},
            {'h', type_t::none},
            {'k', type_t::sentence},
            {'P', type_t::sentence},
            {'@', type_t::sentence}
        };

        /// local variable
        char working_option;

        /// functions
        char	    **ft_go_after_option(char **av);
        int         fillOption(char *s);
        int         checkWorkingArg(char *s);
        void        parse(int ac, char **av);
        std::string resolve_dns(const std::string& domain_name);
    public:
        /// configs
        std::string config_path;
        std::string log_path;
        std::string crytage_key;
        std::string master_password;
        std::string ip;
        int port;

        Option();
        char	**configure(int ac, char **av);
        void usage();
};

#endif