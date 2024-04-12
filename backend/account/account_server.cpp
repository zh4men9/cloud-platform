#include "account_server.h"
#include "../network/tcp_client.h"
#include "../common/utils.h"
#include <iostream>
#include <fstream>
#include <filesystem>

const std::string USER_DATA_DIR = "./user_data/";

AccountServer::AccountServer(int port, const std::string& coordinatorHost, int coordinatorPort)
    : server(port), coordinatorClient(new TCPClient(coordinatorHost, coordinatorPort)) {
    server.start();
    loadUserData();
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
    std::string userDataFile = USER_DATA_DIR + username + ".txt";
    std::ifstream file(userDataFile);

    std::cout << "Open file " << userDataFile << std::endl;

    if (file.is_open()) {
        std::string data;
        std::string line;
        while (std::getline(file, line)) {
            data += line + "\n";
        }
        file.close();

        std::cout << "Read data:\n" << data << std::endl;
        return Utils::deserializeUser(data);
    } else {
        std::cout << "Read file failed" << std::endl;
    }

    return User();
}

bool AccountServer::createUser(const std::string& username, const std::string& password, const std::string& email) {
    std::string userDataFile = USER_DATA_DIR + username + ".txt";
    if (!std::filesystem::exists(userDataFile)) {
        User user(username, password, email);
        std::ofstream file(userDataFile);
        if (file.is_open()) {
            file << Utils::serializeUser(user);
            file.close();
            return true;
        }
    }
    return false;
}

void AccountServer::loadUserData() {
    if (!std::filesystem::exists(USER_DATA_DIR)) {
        std::filesystem::create_directory(USER_DATA_DIR);
    }

    for (const auto& entry : std::filesystem::directory_iterator(USER_DATA_DIR)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            std::string username = filename.substr(0, filename.length() - 4); // 去掉 ".txt" 后缀
            std::string userDataFile = USER_DATA_DIR + filename;

            std::ifstream file(userDataFile);
            if (file.is_open()) {
                std::string line;
                std::getline(file, line);
                file.close();
                User user = Utils::deserializeUser(line);
                storeUser(user);
            } else {
                std::cerr << "Failed to open user data file: " << userDataFile << std::endl;
            }
        }
    }
}

void AccountServer::storeUser(const User& user) {
    users[user.username] = user;
}