#include "file.h"

File::File(const std::string& name, const std::string& content)
    : name(name), content(content) {
}

File::File(const File& other)
    : name(other.name), content(other.content) {
}

File& File::operator=(const File& other) {
    if (this != &other) {
        name = other.name;
        content = other.content;
    }
    return *this;
}