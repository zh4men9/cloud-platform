#ifndef POP3_SERVER_H
#define POP3_SERVER_H

#include "../network/tcp_server.h"
#include "../network/tcp_client.h"
#include "../common/types.h"
#include "email.h"
#include <unordered_map>
#include <memory>

class POP3Server {
public:
    POP3Server(int port, const std::string& coordinatorHost, int coordinatorPort);
    void run();
    std::vector<Email> getEmails(const std::string& username);
    bool deleteEmail(const std::string& username, size_t index);

private:
    TCPResponse processRequest(const TCPRequest& request);
    void storeEmail(const std::string& username, const Email& email);

    TCPServer server;
    std::unique_ptr<TCPClient> coordinatorClient;
    std::unordered_map<std::string, std::vector<Email>> emails;
};

#endif // POP3_SERVER_H