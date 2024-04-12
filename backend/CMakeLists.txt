cmake_minimum_required(VERSION 3.10)
project(server_cloud_platform)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_COMPILER /usr/bin/g++-9)

# Add include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/network
    ${PROJECT_SOURCE_DIR}/coordinator
    ${PROJECT_SOURCE_DIR}/account
    ${PROJECT_SOURCE_DIR}/email
    ${PROJECT_SOURCE_DIR}/storage_service
    ${PROJECT_SOURCE_DIR}/admin
)

# Add source files
set(SOURCE_FILES
    common/types.h
    common/utils.cpp
    network/tcp_server.cpp
    network/tcp_client.cpp
    coordinator/coordinator.cpp
    coordinator/tablet_manager.cpp
    account/account_server.cpp
    account/user.cpp
    email/email.cpp
    email/smtp_server.cpp
    email/pop3_server.cpp
    storage_service/storage_server.cpp
    storage_service/file.cpp
    admin/admin_server.cpp
    server_main.cpp
)

# Add the executable
add_executable(server_cloud_platform ${SOURCE_FILES})

# Link against necessary libraries
target_link_libraries(server_cloud_platform
    pthread
    rt
)