#include "storage_server.h"
#include "../network/tcp_client.h"
#include "../common/utils.h"
#include <iostream>
#include <filesystem>
#include <fstream>

StorageServer::StorageServer(int port, const std::string& coordinatorHost, int coordinatorPort, size_t numTablets)
    : server(port), coordinatorClient(new TCPClient(coordinatorHost, coordinatorPort)), numTablets(numTablets) {
    server.start();
    createTabletDirectories();
}

void StorageServer::run() {
    while (true) {
        TCPRequest request = server.acceptAndRead();
        TCPResponse response = processRequest(request);
        server.sendResponse(response);
    }
}

TCPResponse StorageServer::processRequest(const TCPRequest& request) {
    TCPResponse response;
    response.success = true;

    if (request.method == "GET_FILE") {
        if (request.args.size() == 1) {
            std::string fileName = request.args[0];
            File file = getFile(fileName);
            response.result = Utils::serializeFile(file);
        } else {
            response.success = false;
            response.result = "INVALID_GET_FILE_REQUEST";
        }
    } else if (request.method == "STORE_FILE") {
        if (request.args.size() == 2) {
            std::string fileName = request.args[0];
            std::string fileData = request.args[1];
            File file(fileName, fileData);
            if (storeFile(file)) {
                response.result = "FILE_STORED";
            } else {
                response.success = false;
                response.result = "FAILED_TO_STORE_FILE";
            }
        } else {
            response.success = false;
            response.result = "INVALID_STORE_FILE_REQUEST";
        }
    } else if (request.method == "DELETE_FILE") {
        if (request.args.size() == 1) {
            std::string fileName = request.args[0];
            if (deleteFile(fileName)) {
                response.result = "FILE_DELETED";
            } else {
                response.success = false;
                response.result = "FAILED_TO_DELETE_FILE";
            }
        } else {
            response.success = false;
            response.result = "INVALID_DELETE_FILE_REQUEST";
        }
    } else {
        response.success = false;
        response.result = "UNKNOWN_REQUEST_METHOD";
    }

    return response;
}

File StorageServer::getFile(const std::string& name) {
    auto it = files.find(name);
    if (it != files.end()) {
        return it->second;
    }
    
    File file = getFileFromTablet(name);
    if (!file.name.empty()) {
        files[name] = file;
    }
    return file;
}

bool StorageServer::storeFile(const File& file) {
    files[file.name] = file;
    storeFileInTablet(file);
    return true;
}

bool StorageServer::deleteFile(const std::string& name) {
    auto it = files.find(name);
    if (it != files.end()) {
        files.erase(it);
        return deleteFileFromTablet(name);
    }
    return false;
}

void StorageServer::storeFileInTablet(const File& file) {
    int tabletIndex = std::hash<std::string>()(file.name) % numTablets;
    std::filesystem::path tabletDir = "tablet_" + std::to_string(tabletIndex);
    std::filesystem::path filePath = tabletDir / file.name;

    std::ofstream fileStream(filePath, std::ios::binary);
    if (fileStream.is_open()) {
        fileStream.write(file.data.c_str(), file.data.length());
        fileStream.close();
        std::cout << "Stored file '" << file.name << "' in tablet " << tabletIndex << std::endl;
    } else {
        std::cerr << "Failed to store file '" << file.name << "' in tablet " << tabletIndex << std::endl;
    }
}

File StorageServer::getFileFromTablet(const std::string& name) {
    int tabletIndex = std::hash<std::string>()(name) % numTablets;
    std::filesystem::path tabletDir = "tablet_" + std::to_string(tabletIndex);
    std::filesystem::path filePath = tabletDir / name;

    if (std::filesystem::exists(filePath)) {
        std::ifstream fileStream(filePath, std::ios::binary);
        if (fileStream.is_open()) {
            std::string fileData((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
            fileStream.close();
            std::cout << "Retrieved file '" << name << "' from tablet " << tabletIndex << std::endl;
            return File(name, fileData);
        }
    }

    return File();
}

bool StorageServer::deleteFileFromTablet(const std::string& name) {
    int tabletIndex = std::hash<std::string>()(name) % numTablets;
    std::filesystem::path tabletDir = "tablet_" + std::to_string(tabletIndex);
    std::filesystem::path filePath = tabletDir / name;

    if (std::filesystem::exists(filePath)) {
        std::filesystem::remove(filePath);
        std::cout << "Deleted file '" << name << "' from tablet " << tabletIndex << std::endl;
        return true;
    }

    return false;
}

void StorageServer::createTabletDirectories() {
    for (size_t i = 0; i < numTablets; i++) {
        std::filesystem::path tabletDir = "tablet_" + std::to_string(i);
        if (!std::filesystem::exists(tabletDir)) {
            std::filesystem::create_directory(tabletDir);
            std::cout << "Created tablet directory: " << tabletDir << std::endl;
        }
    }
}