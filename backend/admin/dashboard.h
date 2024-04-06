#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "../network/tcp_client.h"
#include "../common/types.h"

class Dashboard {
public:
    Dashboard(const std::string& adminHost, int adminPort);
    std::string getSystemStatus();
    void controlSystem(const std::string& command);

private:
    std::unique_ptr<TCPClient> adminClient;
};

#endif // DASHBOARD_H