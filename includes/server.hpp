#ifndef SEVER_HPP
#define SEVER_HPP

#include <color.hpp>

#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <functional>
#include <string>
#include <fstream>

class Server {
    #define MAX_CLIENT 3

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
        std::string configure(int port);
        void start();
        void stop();
};


#endif