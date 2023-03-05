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
#include <sys/stat.h>

#include <logging.hpp>

class Server {
    #define MAX_CLIENT 3

    private:
        bool                running;
        int                 server_fd;
        std::vector<int>    client_socket;
        char                buffer[1024];
        struct sockaddr_in  address;
        socklen_t           addrlen;

        int Demonize();
    public:
        std::function<std::string(const char *)> onMessageReceive;
        std::function<std::string(std::string)> decrypter;
        std::function<std::string(std::string)> encrypter;


        Server();
        std::string configure(int port);
        void start();
        void stop();
};


#endif