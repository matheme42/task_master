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
#include <algorithm>

#include <logging.hpp>

class Server {
    #define MAX_CLIENT 3

    private:
        bool                running;
        int                 server_fd;
        std::vector<int>    client_socket;
        std::vector<int>    authenticate_client;
        char                buffer[1024];
        struct sockaddr_in  address;
        socklen_t           addrlen;
        std::string         master_password;


        int Demonize();
    public:
        std::function<std::string(const char *)> onMessageReceive;
        std::function<std::string(std::string)> decrypter;
        std::function<std::string(std::string)> encrypter;

        Server();
        std::string configure(int port, std::string master_password);
        void start();
        void stop();
};


#endif