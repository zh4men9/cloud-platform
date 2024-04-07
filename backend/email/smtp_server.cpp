#include "smtp_server.h"
#include <iostream>

SMTPServer::SMTPServer(int port, const std::string& coordinatorHost, int coordinatorPort)
    : server(port), coordinatorClient(new TCPClient(coordinatorHost, coordinatorPort)) {
    server.start();
}

void SMTPServer::run() {
    while (true) {
        TCPRequest request = server.acceptAndRead();
        TCPResponse response = processRequest(request);
        server.sendResponse(response);
    }
}

TCPResponse SMTPServer::processRequest(const TCPRequest& request) {
    TCPResponse response;
    response.success = true;

    if (request.method == "SEND_EMAIL") {
        if (request.args.size() == 4) {
            std::string from = request.args[0];
            std::string to = request.args[1];
            std::string subject = request.args[2];
            std::string body = request.args[3];

            Email email{ from, to, subject, body };
            if (sendEmail(email)) {
                response.result = "EMAIL_SENT";
            } else {
                response.success = false;
                response.result = "FAILED_TO_SEND_EMAIL";
            }
        } else {
            response.success = false;
            response.result = "INVALID_SEND_EMAIL_REQUEST";
        }
    } else {
        response.success = false;
        response.result = "UNKNOWN_REQUEST_METHOD";
    }

    return response;
}

bool SMTPServer::sendEmail(const Email& email) {
    // Implement the logic to send the email
    std::cout << "Sending email:\n"
              << "From: " << email.from << "\n"
              << "To: " << email.to << "\n"
              << "Subject: " << email.subject << "\n"
              << "Body: " << email.body << std::endl;
    return true;
}