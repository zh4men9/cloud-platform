#include "tcp_client.h"
#include "../common/utils.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

TCPClient::TCPClient(const std::string& host, int port)
    : host(host), port(port) {
}

void TCPClient::connect() {
    // Create the client socket
    socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0) {
        throw std::runtime_error("Error creating socket");
    }

    // Set up the server address
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid server address");
    }

    // Connect to the server
    if (::connect(socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        throw std::runtime_error("Error connecting to server");
    }
}

void TCPClient::send(const std::string& data) {
    Utils::writeToSocket(socket, data);
}

std::string TCPClient::receive() {
    return Utils::readFromSocket(socket);
}