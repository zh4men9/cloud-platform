#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "../common/types.h"
#include <string>

class TCPServer {
public:
    TCPServer(int port);
    void start();
    void stop();
    TCPRequest acceptAndRead();
    void sendResponse(const TCPResponse& response);

private:
    std::string readFromSocket(int socket);
    void writeToSocket(int socket, const std::string& data);

    int port;
    int serverSocket;
};

TCPRequest parseRequest(const std::string& requestStr);
std::string serializeResponse(const TCPResponse& response);

#endif // TCP_SERVER_H