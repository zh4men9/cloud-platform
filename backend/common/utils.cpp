#include "utils.h"
#include <sstream>
#include <iostream>
#include <unistd.h>

std::vector<std::string> Utils::splitString(const std::string& str, char delimiter) {
    std::vector<std::string> parts;
    std::stringstream ss(str);
    std::string part;
    while (std::getline(ss, part, delimiter)) {
        parts.push_back(part);
    }
    return parts;
}

std::string Utils::readFromSocket(int socket) {
    char buffer[1024];
    std::string data;
    ssize_t bytesRead;
    do {
        bytesRead = read(socket, buffer, sizeof(buffer));
        if (bytesRead > 0) {
            data.append(buffer, bytesRead);
        }
    } while (bytesRead > 0);
    return data;
}

void Utils::writeToSocket(int socket, const std::string& data) {
    ssize_t totalBytesSent = 0;
    ssize_t bytesSent;
    while (totalBytesSent < data.length()) {
        bytesSent = write(socket, data.c_str() + totalBytesSent, data.length() - totalBytesSent);
        if (bytesSent < 0) {
            throw std::runtime_error("Error writing to socket");
        }
        totalBytesSent += bytesSent;
    }
}

std::string Utils::serializeUser(const User& user) {
    std::stringstream ss;
    ss << user.username << "\n" << user.password << "\n" << user.email;
    return ss.str();
}

std::string Utils::serializeFile(const File& file) {
    std::stringstream ss;
    ss << file.name << "\n" << file.data;
    return ss.str();
}

std::string Utils::serializeEmail(const Email& email) {
    std::stringstream ss;
    ss << email.from << "\n" << email.to << "\n" << email.subject << "\n" << email.body;
    return ss.str();
}

User Utils::deserializeUser(const std::string& str) {
    std::vector<std::string> parts = Utils::splitString(str, '\n');
    if (parts.size() != 3) {
        return User();
    }

    std::string username = parts[0];
    std::string password = parts[1];
    std::string email = parts[2];
    return User(username, password, email);
}