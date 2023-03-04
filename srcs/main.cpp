#include "application.hpp"
#include<signal.h>

Application application;

void sig_handler(int signo)
{
  if (signo == SIGINT) {
    application.sigint();
  } else if (signo == SIGWINCH) {
   application.sigwinch();
  }
}

int main(int ac, char **av) {
    if (geteuid() != 0) {
      fprintf(stderr, "App needs root\n");
      exit(1);
    }
    application.initWithArg(ac, av);

    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("can't catch SIGINT\n");

    if (signal(SIGWINCH, sig_handler) == SIG_ERR)
        printf("can't catch SIGINT\n");


    application.start();
    return (0);
}