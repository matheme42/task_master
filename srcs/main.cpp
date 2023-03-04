#include "application.hpp"
#include<signal.h>

Application application;

/// logging
Tintin_reporter reporter;

void sig_handler(int signo)
{
  if (signo == SIGINT) {
    reporter.signal("sigint");
    application.sigint();
  } else if (signo == SIGWINCH) {
    reporter.signal("sigwinch");
    application.sigwinch();
  } else if (signo == SIGSEGV) {
    reporter.signal("sigsegv");
    application.stop();
  } else if (signo == SIGTERM) {
    reporter.signal("sigterm");
    application.stop();
  } else if (signo == SIGQUIT) {
    reporter.signal("sigquit");
    application.stop();
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
        printf("can't catch SIGWINCH\n");

    if (signal(SIGSEGV, sig_handler) == SIG_ERR)
        printf("can't catch SIGSEGV\n");

    if (signal(SIGTERM , sig_handler) == SIG_ERR)
        printf("can't catch SIGTERM\n");
    
    if (signal(SIGQUIT , sig_handler) == SIG_ERR)
        printf("can't catch SIGQUIT\n");

    application.start();
    return (0);
}