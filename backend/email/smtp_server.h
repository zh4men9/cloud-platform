#ifndef SMTP_SERVER_H
#define SMTP_SERVER_H

#include "../network/tcp_server.h"
#include "../network/tcp_client.h"
#include "email.h"
#include <string>
#include <memory>

class SMTPServer {
public:
    SMTPServer(int port, const std::string& coordinatorHost, int coordinatorPort);
    void run();
    bool sendEmail(const Email& email);

private:
    TCPResponse processRequest(const TCPRequest& request);

    TCPServer server;
    std::unique_ptr<TCPClient> coordinatorClient;
};

#endif // SMTP_SERVER_H