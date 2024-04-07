#ifndef STORAGE_SERVER_H
#define STORAGE_SERVER_H

#include "../network/tcp_server.h"
#include "../network/tcp_client.h"
#include "file.h"
#include <unordered_map>
#include <memory>

class StorageServer {
public:
    StorageServer(int port, const std::string& coordinatorHost, int coordinatorPort, size_t numTablets);
    void run();
    File getFile(const std::string& name);
    bool storeFile(const File& file);
    bool deleteFile(const std::string& name);

private:
    TCPResponse processRequest(const TCPRequest& request);
    void storeFileInTablet(const File& file);
    File getFileFromTablet(const std::string& name);
    bool deleteFileFromTablet(const std::string& name);
    void createTabletDirectories();

    TCPServer server;
    std::unique_ptr<TCPClient> coordinatorClient;
    std::unordered_map<std::string, File> files;
    size_t numTablets;
};

#endif // STORAGE_SERVER_H