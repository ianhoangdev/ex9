#include <iostream>
#include <string>
#include <cstring> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void handleError(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./server <port_number>" << std::endl;
        return 1;
    }

    int port = atoi(argv[1]); // 

    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        handleError("ERROR opening socket");
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        handleError("ERROR setting socket options");
    }

    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        handleError("ERROR on binding");
    }

    std::cout << "Server starting on port " << port << "... Waiting for connections." << std::endl;
    listen(server_fd, 5); // 

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
        handleError("ERROR on accept");
    }
    std::cout << "Client connected!" << std::endl;

    std::memset(buffer, 0, 1024);
    int n = read(client_fd, buffer, 1023);
    if (n < 0) {
        handleError("ERROR reading from socket");
    }
    std::cout << "Client said: " << buffer << std::endl;

    std::string server_msg = "Welcome to the server running on REPTILIAN"; // 
    n = write(client_fd, server_msg.c_str(), server_msg.length());
    if (n < 0) {
        handleError("ERROR writing to socket");
    }

    close(client_fd);
    close(server_fd);
    return 0;
}