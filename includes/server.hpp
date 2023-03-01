#ifndef SEVER_HPP
#define SEVER_HPP

#include<signal.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <functional>
#include <string>

class Server {
    private:
        bool                running;
        int                 server_fd;
        std::vector<int>    client_socket;
        char                buffer[1024];
        struct sockaddr_in  address;
        socklen_t           addrlen;

    public:
        std::function<std::string(const char *)> onMessageReceive;

        Server();
        void start(int port);
        void stop();
};


#endif