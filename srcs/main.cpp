#include "application.hpp"
#include<signal.h>

Application application;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    application.stop();
}

int main(int ac, char **av) {
    application.initWithArg(ac, av);


    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");

    application.start();
    return (0);
}