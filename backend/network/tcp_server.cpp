#include "tcp_server.h"
#include "../common/types.h"
#include "../common/utils.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sstream>

TCPServer::TCPServer(int port)
    : port(port), serverSocket(-1) {
}

void TCPServer::start() {
    // Create the server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw std::runtime_error("Failed to create server socket");
    }

    // Bind the socket to the specified port
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        throw std::runtime_error("Failed to bind server socket");
    }

    // Start listening for incoming connections
    if (listen(serverSocket, 10) < 0) {
        throw std::runtime_error("Failed to start listening on server socket");
    }
}

void TCPServer::stop() {
    close(serverSocket);
}

TCPRequest TCPServer::acceptAndRead() {
    // Accept a client connection
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket < 0) {
        throw std::runtime_error("Failed to accept client connection");
    }

    // Read the request from the client
    std::string requestStr = readFromSocket(clientSocket);
    close(clientSocket);

    // Parse the request
    return parseRequest(requestStr);
}

void TCPServer::sendResponse(const TCPResponse& response) {
    // Create the client socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        throw std::runtime_error("Failed to create client socket");
    }

    // Connect to the client
    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    clientAddr.sin_port = htons(0);

    if (connect(clientSocket, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) < 0) {
        throw std::runtime_error("Failed to connect to client");
    }

    // Send the response
    std::string responseStr = serializeResponse(response);
    writeToSocket(clientSocket, responseStr);
    close(clientSocket);
}

std::string TCPServer::readFromSocket(int socket) {
    std::stringstream ss;
    char buffer[1024];
    ssize_t bytesRead;

    while ((bytesRead = read(socket, buffer, sizeof(buffer))) > 0) {
        ss.write(buffer, bytesRead);
    }

    if (bytesRead < 0) {
        throw std::runtime_error("Error reading from socket");
    }

    return ss.str();
}

void TCPServer::writeToSocket(int socket, const std::string& data) {
    const char* ptr = data.c_str();
    size_t remaining = data.size();

    while (remaining > 0) {
        ssize_t bytesSent = write(socket, ptr, remaining);
        if (bytesSent < 0) {
            throw std::runtime_error("Error writing to socket");
        }
        ptr += bytesSent;
        remaining -= bytesSent;
    }
}

TCPRequest parseRequest(const std::string& requestStr) {
    std::istringstream iss(requestStr);
    std::string line;
    std::getline(iss, line);
    
    TCPRequest request;
    request.method = line;
    
    while (std::getline(iss, line)) {
        if (!line.empty()) {
            request.args.push_back(line);
        }
    }
    
    return request;
}

std::string serializeResponse(const TCPResponse& response) {
    std::stringstream ss;
    ss << (response.success ? "SUCCESS" : "FAILURE") << "\n";
    ss << response.result;
    return ss.str();
}