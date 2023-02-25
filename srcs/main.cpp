#include "application.hpp"

int main(int ac, char **av) {
    Application application;

    application.initWithArg(ac, av);
    application.start();
    return (0);
}