#include "server.hpp"

void Server::stop() {
    running = false;
    fclose(stdin);
}

Server::Server() {
    bzero(buffer, sizeof(buffer));
    server_fd = 0;
}

void Server::start(int port) {
    running = true;
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    fcntl(server_fd, F_SETFL, O_NONBLOCK);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

   // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    int     ret;
    std::string command_ret;
    while (running) {
        int localNewSocket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (localNewSocket > 0) {
            client_socket.push_back(localNewSocket);
            fcntl(localNewSocket, F_SETFL, O_NONBLOCK);
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
        shutdown(socket, SHUT_RDWR);
        close(socket);
    }
    // closing the listening socket
    if (server_fd > 0) {
        shutdown(server_fd, SHUT_RDWR);
        close(server_fd);
    }
}

