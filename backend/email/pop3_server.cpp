#include "pop3_server.h"
#include "../network/tcp_client.h"
#include "../common/utils.h"
#include <iostream>

POP3Server::POP3Server(int port, const std::string& coordinatorHost, int coordinatorPort)
    : server(port), coordinatorClient(new TCPClient(coordinatorHost, coordinatorPort)) {
    // Start the TCP server
    server.start();
}

void POP3Server::run() {
    while (true) {
        // Accept client connection and read the request
        TCPRequest request = server.acceptAndRead();

        // Process the request
        TCPResponse response = processRequest(request);

        // Send the response
        server.sendResponse(response);
    }
}

TCPResponse POP3Server::processRequest(const TCPRequest& request) {
    TCPResponse response;
    response.success = true;

    if (request.method == "GET_EMAILS") {
        if (request.args.size() == 1) {
            std::string username = request.args[0];
            std::vector<Email> userEmails = getEmails(username);
            std::string emailsStr;
            for (const auto& email : userEmails) {
                emailsStr += Utils::serializeEmail(email) + "\n";
            }
            response.result = emailsStr;
        } else {
            response.success = false;
            response.result = "INVALID_GET_EMAILS_REQUEST";
        }
    } else if (request.method == "DELETE_EMAIL") {
        if (request.args.size() == 2) {
            std::string username = request.args[0];
            size_t index = std::stoul(request.args[1]);
            if (deleteEmail(username, index)) {
                response.result = "EMAIL_DELETED";
            } else {
                response.success = false;
                response.result = "FAILED_TO_DELETE_EMAIL";
            }
        } else {
            response.success = false;
            response.result = "INVALID_DELETE_EMAIL_REQUEST";
        }
    } else {
        response.success = false;
        response.result = "UNKNOWN_REQUEST_METHOD";
    }

    return response;
}

std::vector<Email> POP3Server::getEmails(const std::string& username) {
    auto it = emails.find(username);
    if (it != emails.end()) {
        return it->second;
    }
    return {};
}

bool POP3Server::deleteEmail(const std::string& username, size_t index) {
    auto it = emails.find(username);
    if (it != emails.end() && index < it->second.size()) {
        it->second.erase(it->second.begin() + index);
        return true;
    }
    return false;
}

void POP3Server::storeEmail(const std::string& username, const Email& email) {
    emails[username].push_back(email);
}