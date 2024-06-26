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
    std::cout << "Coordinator started, listening on port " << server.getPort() << std::endl;

    std::vector<std::thread> workerThreads;
    for (int i = 0; i < 4; ++i) {
        workerThreads.emplace_back([this]() {
            while (true) {
                try {
                    std::cout << "Worker thread " << std::this_thread::get_id() << " waiting for request..." << std::endl;
                    TCPRequest request = server.acceptAndRead();
                    std::cout << "Worker thread " << std::this_thread::get_id() << " received request: method=" << request.method << ", args=" << std::endl;
                    for (const auto& arg : request.args) {
                        std::cout << "  - " << arg << std::endl;
                    }

                    TCPResponse response = processRequest(request);

                    std::cout << "Worker thread " << std::this_thread::get_id() << " sending response: success=" << response.success << ", result=" << response.result << std::endl;
                    server.sendResponse(response);
                } catch (const std::exception& e) {
                    std::cerr << "Error in worker thread " << std::this_thread::get_id() << ": " << e.what() << std::endl;
                }
            }
        });
    }

    for (auto& thread : workerThreads) {
        thread.join();
    }
}

TCPResponse Coordinator::processRequest(const TCPRequest& request) {

    std::cout << "In Coordinator::processRequest" << std::endl;
    std::cout << "Received request: method=" << request.method << ", args=" << std::endl;
    for (const auto& arg : request.args) {
        std::cout << "  - " << arg << std::endl;
    }

    TCPResponse response;
    response.success = true;
    response.clientSocket = request.clientSocket;

    if (request.method == "GET_ACCOUNT") {
        std::cout << "In GET_ACCOUNT" << std::endl;
        if (request.args.size() == 1) {
            std::string username = request.args[0];
            User user = accountServer->getUser(username);
            response.result = Utils::serializeUser(user);
        } else {
            response.success = false;
            response.result = "INVALID_GET_ACCOUNT_REQUEST";
        }
    } else if (request.method == "CREATE_ACCOUNT") {
        std::cout << "In CREATE_ACCOUNT" << std::endl;
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
        std::cout << "In SEND_EMAIL" << std::endl;
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
        std::cout << "In GET_FILE" << std::endl;
        if (request.args.size() == 1) {
            std::string fileName = request.args[0];
            File file = storageServer->getFile(fileName);
            response.result = Utils::serializeFile(file);
        } else {
            response.success = false;
            response.result = "INVALID_GET_FILE_REQUEST";
        }
    } else if (request.method == "STORE_FILE") {
        std::cout << "In STORE_FILE" << std::endl;
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
        std::cout << "In DELETE_FILE" << std::endl;
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
        std::cout << "In GET_SYSTEM_STATUS" << std::endl;
        std::string status = adminServer->getSystemStatus();
        response.result = status;
    } else if (request.method == "CONTROL_SYSTEM") {
        std::cout << "In CONTROL_SYSTEM" << std::endl;
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