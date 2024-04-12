#include "account_server.h"
#include "../network/tcp_client.h"
#include "../common/utils.h"
#include <iostream>

AccountServer::AccountServer(int port, const std::string& coordinatorHost, int coordinatorPort)
    : server(port), coordinatorClient(new TCPClient(coordinatorHost, coordinatorPort)) {
    server.start();
}

void AccountServer::run() {
    while (true) {
        TCPRequest request = server.acceptAndRead();
        TCPResponse response = processRequest(request);
        server.sendResponse(response);
    }
}

TCPResponse AccountServer::processRequest(const TCPRequest& request) {

    std::cout << "Received request: method=" << request.method << ", args=" << std::endl;
    for (const auto& arg : request.args) {
        std::cout << "  - " << arg << std::endl;
    }

    TCPResponse response;
    response.success = true;

    if (request.method == "GET_USER") {
        if (request.args.size() == 1) {
            std::string username = request.args[0];
            User user = getUser(username);
            response.result = Utils::serializeUser(user);
        } else {
            response.success = false;
            response.result = "INVALID_GET_USER_REQUEST";
        }
    } else if (request.method == "CREATE_USER") {
        if (request.args.size() == 3) {
            std::string username = request.args[0];
            std::string password = request.args[1];
            std::string email = request.args[2];
            if (createUser(username, password, email)) {
                response.result = "USER_CREATED";
            } else {
                response.success = false;
                response.result = "FAILED_TO_CREATE_USER";
            }
        } else {
            response.success = false;
            response.result = "INVALID_CREATE_USER_REQUEST";
        }
    } else {
        response.success = false;
        response.result = "UNKNOWN_REQUEST_METHOD";
    }

    std::cout << "Sending response: success=" << response.success << ", result=" << response.result << std::endl;
    
    return response;
}

User AccountServer::getUser(const std::string& username) {
    auto it = users.find(username);
    if (it != users.end()) {
        return it->second;
    }
    return User();
}

bool AccountServer::createUser(const std::string& username, const std::string& password, const std::string& email) {
    if (users.find(username) == users.end()) {
        User user(username, password, email);
        storeUser(user);
        return true;
    }
    return false;
}

void AccountServer::storeUser(const User& user) {
    users[user.username] = user;
}