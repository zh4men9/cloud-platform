#include "smtp_server.h"
#include "../network/tcp_client.h"
#include "../common/utils.h"
#include <iostream>

SMTPServer::SMTPServer(int port, const std::string& coordinatorHost, int coordinatorPort)
    : server(port), coordinatorClient(new TCPClient(coordinatorHost, coordinatorPort)) {
    // Start the TCP server
    server.start();
}

void SMTPServer::run() {
    while (true) {
        // Accept client connection and read the request
        TCPRequest request = server.acceptAndRead();

        // Process the request
        TCPResponse response = processRequest(request);

        // Send the response
        server.sendResponse(response);
    }
}

TCPResponse SMTPServer::processRequest(const TCPRequest& request) {
    TCPResponse response;
    response.success = true;

    if (request.method == "SEND_EMAIL") {
        if (request.args.size() == 4) {
            Email email = {
                .from = request.args[0],
                .to = request.args[1],
                .subject = request.args[2],
                .body = request.args[3]
            };
            if (sendEmail(email)) {
                response.result = "EMAIL_SENT";
            } else {
                response.success = false;
                response.result = "FAILED_TO_SEND_EMAIL";
            }
        } else {
            response.success = false;
            response.result = "INVALID_EMAIL_REQUEST";
        }
    } else if (request.method == "GET_EMAILS") {
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
    } else {
        response.success = false;
        response.result = "UNKNOWN_REQUEST_METHOD";
    }

    return response;
}

bool SMTPServer::sendEmail(const Email& email) {
    storeEmail(email);
    coordinatorClient->send("SEND_EMAIL " + email.from + " " + email.to + " " + email.subject + " " + email.body);
    // Implement actual email sending logic
    return true;
}

void SMTPServer::storeEmail(const Email& email) {
    emails[email.to].push_back(email);
}

std::vector<Email> SMTPServer::getEmails(const std::string& username) {
    auto it = emails.find(username);
    if (it != emails.end()) {
        return it->second;
    }
    return {};
}