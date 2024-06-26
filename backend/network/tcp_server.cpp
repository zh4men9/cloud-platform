#include "tcp_server.h"
#include "../common/types.h"
#include "../common/utils.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <system_error>
#include <cerrno>

TCPServer::TCPServer(int port)
    : port(port), serverSocket(-1) {
}

TCPServer::~TCPServer() {
    if (serverSocket != -1) {
        close(serverSocket);
    }
}

void TCPServer::start() {
    int optval = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // Create the server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw std::runtime_error("Failed to create server socket");
    }

    std::cout << "Create server socket success!" << std::endl;

    // Bind the socket to the specified port
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        int errorCode = errno;
        std::cerr << "Failed to bind server socket, error code: " << errorCode << std::endl;
        throw std::system_error(errorCode, std::system_category(), "Failed to bind server socket");
    }

    std::cout << "Bind server socket success in port " << port << " success!" << std::endl;

    // Start listening for incoming connections
    if (listen(serverSocket, 10) < 0) {
        throw std::runtime_error("Failed to start listening on server socket");
    }

    std::cout << "Start a server!" << std::endl;
}

void TCPServer::stop() {
    close(serverSocket);
}

TCPRequest TCPServer::acceptAndRead() {
    std::cout << "Waiting for client connection..." << std::endl;

    // Accept a client connection
    int clientSocket;
    {
        std::lock_guard<std::mutex> lock(socketMutex);
        clientSocket = accept(serverSocket, nullptr, nullptr);
    }
    if (clientSocket < 0) {
        std::cerr << "Failed to accept client connection: " << std::strerror(errno) << std::endl;
        throw std::system_error(errno, std::system_category(), "Failed to accept client connection");
    }

    std::cout << "Client connection accepted" << std::endl;
    
    // Read the request from the client
    std::string requestStr = readFromSocket(clientSocket);

    // Parse the request
    TCPRequest request = parseRequest(requestStr);

    // Save the client socket for later use
    request.clientSocket = clientSocket;

    return request;
}

void TCPServer::sendResponse(const TCPResponse& response) {
    std::cout << "In sendResponse" << std::endl;

    // Send the response
    int clientSocket = response.clientSocket;
    std::string responseStr = serializeResponse(response);
    writeToSocket(clientSocket, responseStr);
    close(clientSocket);

    std::cout << "End sendResponse" << std::endl;
}

std::string TCPServer::readFromSocket(int socket) {
    std::cout << "Start to read from socket." << std::endl;

    std::stringstream ss;
    char buffer[4096];  // 假设 4096 足够大
    ssize_t bytesRead;
    size_t totalBytesRead = 0;

    while (true) {
        std::cout << "Calling read(), waiting for data..." << std::endl;
        bytesRead = read(socket, buffer, sizeof(buffer));
        std::cout << "Read " << bytesRead << " bytes from socket" << std::endl;
        if (bytesRead > 0) {
            ss.write(buffer, bytesRead);
            totalBytesRead += bytesRead;
            break;
        } else if (bytesRead == 0) {
            std::cout << "Client closed the connection" << std::endl;
            break; // 客户端关闭了连接
        } else if (bytesRead < 0 && errno == EAGAIN) {
            // 非阻塞模式下,read()返回-1且errno为EAGAIN,表示暂时没有更多数据可读
            std::cout << "No more data to read, waiting 10ms..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } else {
            // 其他错误情况
            std::cout << "Error reading from socket, errno: " << errno << std::endl;
            throw std::system_error(errno, std::system_category(), "Error reading from socket");
        }
    }

    std::cout << "End to read from socket. Total bytes read: " << totalBytesRead << std::endl;
    std::cout << "Read this content: " << ss.str() << std::endl;

    return ss.str();
}

void TCPServer::writeToSocket(int socket, const std::string& data) {
    const char* ptr = data.c_str();
    size_t remaining = data.size();

    while (remaining > 0) {
        ssize_t bytesSent = write(socket, ptr, remaining);
        if (bytesSent < 0) {
            throw std::system_error(errno, std::system_category(), "Error writing to socket");
        }
        ptr += bytesSent;
        remaining -= bytesSent;
    }
}

TCPRequest parseRequest(const std::string& requestStr) {
    std::cout << "In parseRequest" << std::endl;
    
    TCPRequest request;
    std::vector<std::string> lines;
    std::istringstream iss(requestStr);
    std::string line;
    const std::string delim = "|||";

    // 将请求字符串分割为多行
    while (std::getline(iss, line, delim[0])) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }

    std::cout << "lines.size():" << lines.size() << std::endl;

    // 逐行处理
    for (const auto& l : lines) {
        std::cout << l << std::endl;
        size_t pos = l.find_first_of(':');
        if (pos != std::string::npos) {
            std::string key = l.substr(0, pos);
            std::string value = l.substr(pos + 1);
            if (key == "method") {
                request.method = value;
            } else {
                request.args.push_back(value);
            }
        } else{
            request.args.push_back(l);
        }
    }

    std::cout << "End parseRequest" << std::endl;

    return request;
}

std::string serializeResponse(const TCPResponse& response) {
    std::cout << "In serializeResponse" << std::endl;
    std::cout << "response.success: " << response.success << std::endl;

    std::stringstream ss;
    ss << (response.success ? 1 : 0) << "\n";
    ss << response.result;

    std::cout << "ss.str() " << ss.str() << std::endl;

    std::cout << "End serializeResponse" << std::endl;
    return ss.str();
}