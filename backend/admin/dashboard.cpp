#include "dashboard.h"
#include "../network/tcp_client.h"
#include "../common/utils.h"

Dashboard::Dashboard(const std::string& adminHost, int adminPort)
    : adminClient(new TCPClient(adminHost, adminPort)) {
}

std::string Dashboard::getSystemStatus() {
    try {
        adminClient->connect();
        adminClient->send("GET_STATUS");
        TCPResponse response = adminClient->receive();
        if (response.success) {
            return response.result;
        } else {
            return "Failed to retrieve system status";
        }
    } catch (const std::exception& e) {
        return "Error retrieving system status: " + std::string(e.what());
    }
}

void Dashboard::controlSystem(const std::string& command) {
    try {
        adminClient->connect();
        adminClient->send("CONTROL " + command);
        TCPResponse response = adminClient->receive();
        if (!response.success) {
            std::cerr << "Failed to control the system: " << response.result << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error controlling the system: " << e.what() << std::endl;
    }
}