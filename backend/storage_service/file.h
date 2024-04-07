#ifndef FILE_H
#define FILE_H

#include <string>

struct File {
    std::string name;
    std::string data;

    File() = default;
    File(const std::string& name, const std::string& data)
        : name(name), data(data) {}
};

#endif // FILE_H