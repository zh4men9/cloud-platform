#include "admin_server.h"
#include "../network/tcp_client.h"
#include "../common/utils.h"
#include <iostream>

AdminServer::AdminServer(int port, const std::string& coordinatorHost, int coordinatorPort, const std::string& accountHost, int accountPort, const std::string& emailHost, int emailPort, const std::string& storageHost, int storagePort)
    : server(port),
      coordinatorClient(new TCPClient(coordinatorHost, coordinatorPort)),
      accountClient(new TCPClient(accountHost, accountPort)),
      emailClient(new TCPClient(emailHost, emailPort)),
      storageClient(new TCPClient(storageHost, storagePort)) {
    // Start the TCP server
    server.start();
}

void AdminServer::run() {
    while (true) {
        // Accept client connection and read the request
        TCPRequest request = server.acceptAndRead();

        // Process the request
        TCPResponse response = processRequest(request);

        // Send the response
        server.sendResponse(response);
    }
}

TCPResponse AdminServer::processRequest(const TCPRequest& request) {
    TCPResponse response;
    response.success = true;

    if (request.method == "GET_STATUS") {
        std::string status = getSystemStatus();
        response.result = status;
    } else if (request.method == "CONTROL") {
        if (request.args.size() == 1) {
            std::string command = request.args[0];
            controlSystem(command);
            response.result = "SYSTEM_CONTROLLED";
        } else {
            response.success = false;
            response.result = "INVALID_CONTROL_REQUEST";
        }
    } else {
        response.success = false;
        response.result = "UNKNOWN_REQUEST_METHOD";
    }

    return response;
}

std::string AdminServer::getSystemStatus() {
    // Retrieve system status from the various components
    std::string status = "System Status:\n";
    status += "Coordinator: OK\n";
    status += "Account Server: OK\n";
    status += "Email Server: OK\n";
    status += "Storage Server: OK\n";
    return status;
}

void AdminServer::controlSystem(const std::string& command) {
    // Control the system by sending commands to the various components
    if (command == "restart_coordinator") {
        coordinatorClient->send("RESTART");
    } else if (command == "restart_account_server") {
        accountClient->send("RESTART");
    } else if (command == "restart_email_server") {
        emailClient->send("RESTART");
    } else if (command == "restart_storage_server") {
        storageClient->send("RESTART");
    } else {
        std::cerr << "Invalid control command: " << command << std::endl;
    }
}