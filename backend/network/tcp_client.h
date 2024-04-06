#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include "../common/types.h"
#include <string>

class TCPClient {
public:
    TCPClient(const std::string& host, int port);
    void connect();
    void send(const std::string& data);
    std::string receive();

private:
    std::string host;
    int port;
    int socket;
};

#endif // TCP_CLIENT_H