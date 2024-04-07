#include "coordinator.h"
#include "../network/tcp_client.h"
#include "../common/utils.h"
#include <iostream>

Coordinator::Coordinator(int port, const std::string& storageHost, int storagePort,
                         const std::string& accountHost, int accountPort,
                         const std::string& emailHost, int emailPort,
                         const std::string& storageServiceHost, int storageServicePort,
                         const std::string& adminHost, int adminPort)
    : server(port),
      storageClient(new TCPClient(storageHost, storagePort)),
      accountServer(new AccountServer(accountPort, storageHost, storagePort)),
      smtpServer(new SMTPServer(emailPort, storageHost, storagePort)),
      storageServer(new StorageServer(storageServicePort, storageHost, storagePort, 10)),
      adminServer(new AdminServer(adminPort, storageHost, storagePort, accountHost, accountPort, emailHost, emailPort, storageServiceHost, storageServicePort)) {
    server.start();
}

void Coordinator::run() {
    while (true) {
        // Accept client connection and read the request
        TCPRequest request = server.acceptAndRead();

        // Process the request
        TCPResponse response = processRequest(request);

        // Send the response
        server.sendResponse(response);
    }
}

TCPResponse Coordinator::processRequest(const TCPRequest& request) {
    TCPResponse response;
    response.success = true;

    if (request.method == "GET_ACCOUNT") {
        if (request.args.size() == 1) {
            std::string username = request.args[0];
            User user = accountServer->getUser(username);
            response.result = Utils::serializeUser(user);
        } else {
            response.success = false;
            response.result = "INVALID_GET_ACCOUNT_REQUEST";
        }
    } else if (request.method == "CREATE_ACCOUNT") {
        if (request.args.size() == 3) {
            std::string username = request.args[0];
            std::string password = request.args[1];
            std::string email = request.args[2];
            if (accountServer->createUser(username, password, email)) {
                response.result = "ACCOUNT_CREATED";
            } else {
                response.success = false;
                response.result = "FAILED_TO_CREATE_ACCOUNT";
            }
        } else {
            response.success = false;
            response.result = "INVALID_CREATE_ACCOUNT_REQUEST";
        }
    } else if (request.method == "SEND_EMAIL") {
        if (request.args.size() == 4) {
            Email email = {
                .from = request.args[0],
                .to = request.args[1],
                .subject = request.args[2],
                .body = request.args[3]
            };
            if (smtpServer->sendEmail(email)) {
                response.result = "EMAIL_SENT";
            } else {
                response.success = false;
                response.result = "FAILED_TO_SEND_EMAIL";
            }
        } else {
            response.success = false;
            response.result = "INVALID_SEND_EMAIL_REQUEST";
        }
    } else if (request.method == "GET_FILE") {
        if (request.args.size() == 1) {
            std::string fileName = request.args[0];
            File file = storageServer->getFile(fileName);
            response.result = Utils::serializeFile(file);
        } else {
            response.success = false;
            response.result = "INVALID_GET_FILE_REQUEST";
        }
    } else if (request.method == "STORE_FILE") {
        if (request.args.size() == 2) {
            std::string fileName = request.args[0];
            std::string fileData = request.args[1];
            File file(fileName, fileData);
            if (storageServer->storeFile(file)) {
                response.result = "FILE_STORED";
            } else {
                response.success = false;
                response.result = "FAILED_TO_STORE_FILE";
            }
        } else {
            response.success = false;
            response.result = "INVALID_STORE_FILE_REQUEST";
        }
    } else if (request.method == "DELETE_FILE") {
        if (request.args.size() == 1) {
            std::string fileName = request.args[0];
            if (storageServer->deleteFile(fileName)) {
                response.result = "FILE_DELETED";
            } else {
                response.success = false;
                response.result = "FAILED_TO_DELETE_FILE";
            }
        } else {
            response.success = false;
            response.result = "INVALID_DELETE_FILE_REQUEST";
        }
    } else if (request.method == "GET_SYSTEM_STATUS") {
        std::string status = adminServer->getSystemStatus();
        response.result = status;
    } else if (request.method == "CONTROL_SYSTEM") {
        if (request.args.size() == 1) {
            std::string command = request.args[0];
            adminServer->controlSystem(command);
            response.result = "SYSTEM_CONTROLLED";
        } else {
            response.success = false;
            response.result = "INVALID_CONTROL_SYSTEM_REQUEST";
        }
    } else {
        response.success = false;
        response.result = "UNKNOWN_REQUEST_METHOD";
    }

    return response;
}