#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "../common/types.h"
#include <string>
#include <mutex>
#include <thread>

class TCPServer {
public:
    TCPServer(int port);
    ~TCPServer();

    void start();
    void stop();
    TCPRequest acceptAndRead();
    void sendResponse(const TCPResponse& response);
    int getPort() const { return port; }

private:
    std::string readFromSocket(int socket);
    void writeToSocket(int socket, const std::string& data);

    int port;
    int serverSocket;
    std::mutex socketMutex;
};

TCPRequest parseRequest(const std::string& requestStr);
std::string serializeResponse(const TCPResponse& response);

#endif // TCP_SERVER_H