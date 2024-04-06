#include "storage_node.h"
#include "../network/tcp_client.h"
#include "../common/utils.h"
#include <iostream>

StorageNode::StorageNode(const std::string& coordinatorHost, int coordinatorPort)
    : coordinatorClient(new TCPClient()) {
    // 连接到协调器
    coordinatorClient->connect(coordinatorHost, coordinatorPort);
}

void StorageNode::run() {
    while (true) {
        // 从协调器获取任务
        TCPRequest request = getTaskFromCoordinator();
        if (request.method.empty()) {
            // 如果没有任务,休眠一段时间
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        // 处理任务
        TCPResponse response = processTask(request);

        // 向协调器发送结果
        sendResponseToCoordinator(response);
    }
}

TCPRequest StorageNode::getTaskFromCoordinator() {
    // 向协调器发送请求,获取任务
    std::string request = "GET_TASK";
    coordinatorClient->send(request);
    return TCPRequest { "GET_TASK", {} };
}

TCPResponse StorageNode::processTask(const TCPRequest& request) {
    TCPResponse response;
    response.success = true;

    // 根据请求的方法执行相应的操作
    if (request.method == "PUT") {
        if (request.args.size() == 2) {
            Key key = request.args[0];
            Value value = request.args[1];
            tablets[key] = value;
            std::cout << "Stored key=" << key << ", value=" << value << std::endl;
        } else {
            response.success = false;
            response.result = "Incorrect number of arguments for PUT";
        }
    } else if (request.method == "GET") {
        if (request.args.size() == 1) {
            Key key = request.args[0];
            auto it = tablets.find(key);
            if (it != tablets.end()) {
                response.result = it->second;
            } else {
                response.result = "";
            }
        } else {
            response.success = false;
            response.result = "Incorrect number of arguments for GET";
        }
    } else if (request.method == "DELETE") {
        if (request.args.size() == 1) {
            Key key = request.args[0];
            tablets.erase(key);
            std::cout << "Deleted key=" << key << std::endl;
        } else {
            response.success = false;
            response.result = "Incorrect number of arguments for DELETE";
        }
    } else {
        response.success = false;
        response.result = "Unknown request method";
    }

    return response;
}

void StorageNode::sendResponseToCoordinator(const TCPResponse& response) {
    // 向协调器发送结果
    std::string responseStr = response.success ? "SUCCESS" : "FAILURE";
    responseStr += " " + response.result;
    coordinatorClient->send(responseStr);
}