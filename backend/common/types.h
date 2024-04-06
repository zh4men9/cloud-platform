#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <unordered_map>
#include <vector>

// 定义常用的数据类型
using Key = std::string;
using Value = std::string;
using Tablet = std::unordered_map<Key, Value>;
using TabletID = size_t;
using NodeID = size_t;

// 定义协议相关的数据结构
struct TCPRequest {
    std::string method;
    std::vector<std::string> args;
};

struct TCPResponse {
    bool success;
    std::string result;
};

#endif // TYPES_H