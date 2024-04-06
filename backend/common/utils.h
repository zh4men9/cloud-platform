#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include "types.h"
#include "file.h"
#include "user.h"

class Utils {
public:
    static std::vector<std::string> splitString(const std::string& str, char delimiter);
    static std::string readFromSocket(int socket);
    static void writeToSocket(int socket, const std::string& data);
    static std::string serializeUser(const User& user);
    static std::string serializeFile(const File& file);
};

#endif // UTILS_H