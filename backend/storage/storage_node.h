#ifndef STORAGE_NODE_H
#define STORAGE_NODE_H

#include "../common/types.h"
#include "../network/tcp_client.h"
#include <memory>
#include <unordered_map>

class StorageNode {
public:
    StorageNode(const std::string& coordinatorHost, int coordinatorPort);
    void run();

private:
    TCPRequest getTaskFromCoordinator();
    TCPResponse processTask(const TCPRequest& request);
    void sendResponseToCoordinator(const TCPResponse& response);

    std::unique_ptr<TCPClient> coordinatorClient;
    std::unordered_map<Key, Value> tablets;
};

#endif // STORAGE_NODE_H