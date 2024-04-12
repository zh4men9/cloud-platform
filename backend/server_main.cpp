#include <fstream>
#include <filesystem>

#include "coordinator/coordinator.h"
#include "account/account_server.h"
#include "email/smtp_server.h"
#include "email/pop3_server.h"
#include "storage_service/storage_server.h"
#include "admin/admin_server.h"
#include <iostream>
#include <chrono>
#include <ctime>

int main() {
    // 是否为 DEBUG 模式
    bool DEBUG = true;

    // 获取日志文件路径
    std::string logFileName;
    if (DEBUG) {
        logFileName = "/home/zh4men9/Project/cloud-platform/backend/debug_log.log";
    } else {
        // 获取当前时间戳
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        logFileName = "log_" + std::to_string(now_c) + ".txt";
    }

    // 创建日志文件
    std::ofstream logFile(logFileName, std::ios::out | std::ios::trunc);

    // 重定向 std::cout 到日志文件
    std::streambuf* originalBuffer = std::cout.rdbuf(logFile.rdbuf());

    try {
        // Coordinator settings
        int coordinatorPort = 8000;
        std::string storageHost = "localhost";
        int storagePort = 8001;
        std::string accountHost = "localhost";
        int accountPort = 8002;
        std::string emailHost = "localhost";
        int emailPort = 8003;
        std::string storageServiceHost = "localhost";
        int storageServicePort = 8004;
        std::string adminHost = "localhost";
        int adminPort = 8005;

        // Create and start the Coordinator
        Coordinator coordinator(coordinatorPort, storageHost, storagePort, accountHost, accountPort, emailHost, emailPort, storageServiceHost, storageServicePort, adminHost, adminPort);
        coordinator.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // 还原 std::cout 的缓冲区
    std::cout.rdbuf(originalBuffer);

    return 0;
}