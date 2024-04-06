#include "coordinator/coordinator.h"
#include "account/account_server.h"
#include "email/smtp_server.h"
#include "email/pop3_server.h"
#include "storage_service/storage_server.h"
#include "admin/admin_server.h"
#include <iostream>

int main() {
    try {
        // Coordinator settings
        int coordinatorPort = 8000;
        std::string storageHost = "localhost";
        int storagePort = 8001;
        std::string accountHost = "localhost";
        int accountPort = 8002;
        std::string emailHost = "localhost";
        int emailPort = 8003;
        std::string storageServiceHost = "localhost";
        int storageServicePort = 8004;
        std::string adminHost = "localhost";
        int adminPort = 8005;

        // Create and start the Coordinator
        Coordinator coordinator(coordinatorPort, storageHost, storagePort, accountHost, accountPort, emailHost, emailPort, storageServiceHost, storageServicePort, adminHost, adminPort);
        coordinator.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}