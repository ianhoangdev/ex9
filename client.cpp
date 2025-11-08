#include <iostream>
#include <string>
#include <cstring> 
#include <stdlib.h>   
#include <unistd.h>    
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 

void handleError(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./client <port_number>" << std::endl;
        return 1;
    }
    
    int port = atoi(argv[1]);
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        handleError("ERROR opening socket");
    }

    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        handleError("ERROR invalid address");
    }

    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        handleError("ERROR connecting");
    }
    std::cout << "Connected to server!" << std::endl;

    std::string client_msg = "<Your Name>: <UFID>"; 
    
    int n = write(sock_fd, client_msg.c_str(), client_msg.length());
    if (n < 0) {
        handleError("ERROR writing to socket");
    }

    std::memset(buffer, 0, 1024);
    n = read(sock_fd, buffer, 1023);
    if (n < 0) {
        handleError("ERROR reading from socket");
    }
    std::cout << "Server said: " << buffer << std::endl;

    close(sock_fd);
    return 0;
}