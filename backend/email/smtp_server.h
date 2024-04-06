#ifndef SMTP_SERVER_H
#define SMTP_SERVER_H

#include "../network/tcp_server.h"
#include "../network/tcp_client.h"
#include "../common/types.h"
#include "email.h"
#include <unordered_map>
#include <memory>

class SMTPServer {
public:
    SMTPServer(int port, const std::string& coordinatorHost, int coordinatorPort);
    void run();
    bool sendEmail(const Email& email);

private:
    TCPResponse processRequest(const TCPRequest& request);
    void storeEmail(const Email& email);
    std::vector<Email> getEmails(const std::string& username);

    TCPServer server;
    std::unique_ptr<TCPClient> coordinatorClient;
    std::unordered_map<std::string, std::vector<Email>> emails;
};

#endif // SMTP_SERVER_H