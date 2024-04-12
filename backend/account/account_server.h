#ifndef ACCOUNT_SERVER_H
#define ACCOUNT_SERVER_H

#include "../network/tcp_server.h"
#include "../network/tcp_client.h"
#include "../common/types.h"
#include "user.h"
#include <unordered_map>

class AccountServer {
public:
    AccountServer(int port, const std::string& coordinatorHost, int coordinatorPort);
    void run();

// private:
    TCPResponse processRequest(const TCPRequest& request);
    User getUser(const std::string& username);
    bool createUser(const std::string& username, const std::string& password, const std::string& email);
    void storeUser(const User& user);
    void loadUserData();

    TCPServer server;
    std::unique_ptr<TCPClient> coordinatorClient;
    std::unordered_map<std::string, User> users;
};

#endif // ACCOUNT_SERVER_H