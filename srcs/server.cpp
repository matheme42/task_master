#include "server.hpp"

void Server::stop() {
    running = false;
}

Server::Server() {
    bzero(buffer, sizeof(buffer));
    server_fd = 0;
}

std::string Server::configure(int port) {
    std::string ret;
    running = false;


    if (FILE *file = fopen(LOCKFILE, "r+")) {
        fclose(file);
        ret = ret + LIGHT_RED + "an instance of taskmaster is already running";
        return (ret);
    }

    std::ofstream outfile (LOCKFILE);
    outfile.close();


    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        ret = ret + "socket failed";
        ret = ret + strerror(errno);
        return (ret);
    }

    int opt = 1;
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        ret = ret + "setsockopt";
        ret = ret + strerror(errno);
        return (ret);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

   // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        ret = ret + LIGHT_RED;
        ret = ret + "the port: ";
        ret = ret + std::to_string(port);
        ret = ret + " is already use";
        return (ret);
    } else if (listen(server_fd, 3) < 0) {
        ret = ret + "listen";
        ret = ret + strerror(errno);
        return (ret);
    } else if(daemon(0, 0)) {
        ret = ret + "daemon";
        ret = ret + strerror(errno);
        return (ret);
    }

    fcntl(server_fd, F_SETFL, O_NONBLOCK);
    running = true;
    return (ret);
}

void Server::start() {
    int     ret;
    std::string command_ret;
    while (running) {
        int localNewSocket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (localNewSocket >= 0) {
            if (client_socket.size() == MAX_CLIENT) {
                send(localNewSocket, "server is full", 15, 0);
                close(localNewSocket);
            } else {
                fcntl(localNewSocket, F_SETFL, O_NONBLOCK);
                client_socket.push_back(localNewSocket);
            }
        }

        int socketListLen = client_socket.size();
        for (int i = 0; i < socketListLen; i++) {
            int new_socket = client_socket.at(i);
            if (new_socket > 0 && (ret = read(new_socket, buffer, 1024)) >= 0) {
                if (ret == 0) {
                    client_socket.erase(client_socket.begin() + i--);
                    socketListLen--;
                    continue;
                }
                buffer[ret - 1] = '\0';
                if (onMessageReceive) command_ret = onMessageReceive(buffer);
                if (command_ret[0] != '\0') {
                    send(new_socket, command_ret.c_str(), command_ret.size(), 0);
                } else {
                    send(new_socket, "", 1, 0);
                }
                bzero(buffer, ret);
        }
        }
        usleep(1000);
    }

    while (client_socket.size()) {
        int socket = client_socket.at(0);
        client_socket.erase(client_socket.begin());
        send(socket, "exit", 5, 0);
        shutdown(socket, SHUT_RDWR);
        close(socket);
    }
    // closing the listening socket
    if (server_fd > 0) {
        shutdown(server_fd, SHUT_RDWR);
        close(server_fd);
    }

    remove("/var/lock/taskmaster.lock");
}

