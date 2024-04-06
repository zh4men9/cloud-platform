#include "email.h"

Email::Email(const std::string& from, const std::string& to, const std::string& subject, const std::string& body)
    : from(from), to(to), subject(subject), body(body) {
}

Email::Email(const Email& other)
    : from(other.from), to(other.to), subject(other.subject), body(other.body) {
}

Email& Email::operator=(const Email& other) {
    if (this != &other) {
        from = other.from;
        to = other.to;
        subject = other.subject;
        body = other.body;
    }
    return *this;
}