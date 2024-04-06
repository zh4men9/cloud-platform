#ifndef ADMIN_SERVER_H
#define ADMIN_SERVER_H

#include "../network/tcp_server.h"
#include "../network/tcp_client.h"
#include <string>
#include <memory>

class AdminServer {
public:
    AdminServer(int port, const std::string& coordinatorHost, int coordinatorPort, const std::string& accountHost, int accountPort, const std::string& emailHost, int emailPort, const std::string& storageHost, int storagePort);
    void run();
    std::string getSystemStatus();
    void controlSystem(const std::string& command);

private:
    TCPResponse processRequest(const TCPRequest& request);

    TCPServer server;
    std::unique_ptr<TCPClient> coordinatorClient;
    std::unique_ptr<TCPClient> accountClient;
    std::unique_ptr<TCPClient> emailClient;
    std::unique_ptr<TCPClient> storageClient;
};

#endif // ADMIN_SERVER_H