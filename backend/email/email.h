#ifndef EMAIL_H
#define EMAIL_H

#include <string>

struct Email {
    std::string from;
    std::string to;
    std::string subject;
    std::string body;
};

#endif // EMAIL_H