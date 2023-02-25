#ifndef PROMPT_HPP
#define PROMPT_HPP

#include<signal.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include "functional"

class Prompt {
    #define DEFAULT_PROMPT "---(Taskmaster)-----------------\n"

    private:
        bool                running;
        int                 server_fd;
        std::vector<int>    client_socket;
        char                buffer[1024];
        struct sockaddr_in  address;
        socklen_t           addrlen;

    public:
        std::function<void(const char *)> onMessageReceive;

        Prompt();
        void start(int port);
        void start();
        void stop();
        ~Prompt();
};


#endif