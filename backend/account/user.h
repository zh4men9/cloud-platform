#ifndef USER_H
#define USER_H

#include <string>

struct User {
    std::string username;
    std::string password;
    std::string email;

    User() = default;
    User(const std::string& username, const std::string& password, const std::string& email);
    User(const User& other);
    User& operator=(const User& other);
};

#endif // USER_H