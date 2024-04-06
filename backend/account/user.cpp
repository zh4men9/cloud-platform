#include "user.h"

User::User(const std::string& username, const std::string& password, const std::string& email)
    : username(username), password(password), email(email) {
}

User::User(const User& other)
    : username(other.username), password(other.password), email(other.email) {
}

User& User::operator=(const User& other) {
    if (this != &other) {
        username = other.username;
        password = other.password;
        email = other.email;
    }
    return *this;
}