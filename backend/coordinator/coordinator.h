#ifndef COORDINATOR_H
#define COORDINATOR_H

#include "../network/tcp_server.h"
#include "../network/tcp_client.h"
#include "../account/account_server.h"
#include "../email/smtp_server.h"
#include "../storage_service/storage_server.h"
#include "../admin/admin_server.h"

class Coordinator {
public:
    Coordinator(int port, const std::string& storageHost, int storagePort,
                const std::string& accountHost, int accountPort,
                const std::string& emailHost, int emailPort,
                const std::string& storageServiceHost, int storageServicePort,
                const std::string& adminHost, int adminPort);
    void run();

private:
    TCPResponse processRequest(const TCPRequest& request);

    TCPServer server;
    std::unique_ptr<TCPClient> storageClient;
    std::unique_ptr<AccountServer> accountServer;
    std::unique_ptr<SMTPServer> smtpServer;
    std::unique_ptr<StorageServer> storageServer;
    std::unique_ptr<AdminServer> adminServer;
};

#endif // COORDINATOR_H