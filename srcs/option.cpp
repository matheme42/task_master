#include "option.hpp"


Option::Option() {
    working_option = '\0';
    port = 0;
}


int Option::checkWorkingArg(char *s) {
    int i = 0;

    if (state[working_option] == type_t::number) {
        while (s[i] && isdigit(s[i++])) ;
        if (s[i]) {
            std::cout << "option -" << working_option << ": must be an int" << std::endl;
            exit(EXIT_FAILURE);
        }
        return (i);
    }
    if (state[working_option] == type_t::infile) {
        if (FILE *file = fopen(s, "r")) {
            fclose(file);
        } else {
            if (!strcmp(s, "\0")) {
                std::cout << "option -" << working_option << ": a file must be reffered" << std::endl;
            } else {
                std::cout << "option -" << working_option << ": " << s << " does not exist" << std::endl;
            }
            exit(EXIT_FAILURE); 
        }
        return (strlen(s));
    }
    if (state[working_option] == type_t::outfile) {
        if (FILE *file = fopen(s, "r+")) {
            fclose(file);
            std::string line;
            std::cout << "option -" << working_option << ": a file already exist override / append / cancel ? (Y / A / C): ";
            std::getline(std::cin, line);
            if (line[0] == 'C' || line[0] == 'c') exit(0);
            else if (line[0] == 'Y' || line[0] == 'y') remove(s);
        } else if (FILE *file = fopen(s, "w")) {
            fclose(file);
            remove(s);
        } else {
            if (!strcmp(s, "\0")) {
                std::cout << "option -" << working_option << ": a file path must be reffered" << std::endl;
            } else {
                std::cout << "option -" << working_option << ": " << s << " is not a valid file path" << std::endl;
            }
            exit(EXIT_FAILURE);
        }


        return (strlen(s));
    }
    return i;
}


void Option::usage() {
    std::cout << "taskmaster:" << std::endl;
    std::cout << " ./taskmaster [-l log file path][-p port] config file" << std::endl;
    std::cout << " ./taskmaster [-l log file path][-p port]{-c config file}" << std::endl;

    std::cout << "  required:" << std::endl;
    std::cout << "    a taskmaster config file" << std::endl;

    std::cout << "  options:" << std::endl;
    std::cout << "    -p: port number between 0 and 65535" << std::endl;
    std::cout << "    -c: path to a taskmaster config file" << std::endl;
    std::cout << "    -l: path to log taskmaster commands" << std::endl;
    exit(0);
}

int Option::fillOption(char *s) {
    int i = 0;
    i = checkWorkingArg(s);
    if (working_option == 'h') usage();
    if (working_option == 'c') config_path = s;
    else if (working_option == 'l') log_path = s;
    else if (working_option == 'p') {
        port = atoi(s);
        if (port <= 0 || port > 65535) {
            std::cout << "option -" << working_option << ": must between 0 and 65535" << std::endl;
            exit(EXIT_FAILURE); 
        }
    }

    return (i);
}

/*
** permet de scruter les options
** aussi bien sur le meme arg que sur plusieur arg
*/

void Option::parse(int ac, char **av)
{
	int	i;

	if (((*av)[0] != '-' || !(*av)[1]) && working_option == '\0') {
		working_option = (char)'c';
    }
    i = working_option == '\0' ? 1 : 0;
	while ((*av)[i])
	{
        if (working_option != '\0') {
            i += fillOption(&((*av)[i]));
            working_option = '\0';
            continue;
        }
        working_option = '\0';
        if (state.find((*av)[i]) == state.end()) {
            std::cout << (*av)[i] << " is not an option" << std::endl;
            exit(EXIT_FAILURE);
        }
        working_option = (*av)[i];
        i++;
	}
	if (ac > 1) {
		parse(ac - 1, &av[1]);
        return ;
    }
    if (working_option != '\0') {
        fillOption((char*)"\0");
    }
}

/*
** permet de ce diriger apres les options
** la fonction prend en compte le cas ou il n'y a que le tiret
** et aucune option
** dans ce cas la fonction ne deplace pas le pointeur
*/

char	**Option::ft_go_after_option(char **av)
{
	if (*av && **av == '-' && (*av)[1])
		return (ft_go_after_option(&av[1]));
	return (av);
}

/*
** fonction visible de l'exterieur
** elle permet de recuperer les options via ...
** ...l'adresse d'un int que l'on passe en parametre
** et elle retourne l'argument qui suit les options
*/

char	**Option::configure(int ac, char **av)
{
    if (ac == 1) {
        std::cout << "taskmaster: can't start (run with -h for more informations)" << std::endl;
        exit(0);
    }
	parse(ac - 1, &(av[1]));

    if (config_path.size() == 0 && port == 0) {
        std::cout << "taskmaster: a config file must be reffered with option: -c" << std::endl;
        exit(0);
    }

	return (ft_go_after_option(&(av[1])));
}