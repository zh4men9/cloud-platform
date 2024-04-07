#!/bin/bash

# 创建后端文件结构
mkdir -p backend/common
mkdir -p backend/coordinator
mkdir -p backend/storage
mkdir -p backend/account
mkdir -p backend/email
mkdir -p backend/storage_service
mkdir -p backend/admin
mkdir -p backend/network

touch backend/common/types.h
touch backend/common/utils.cpp
touch backend/common/utils.h

touch backend/coordinator/coordinator.cpp
touch backend/coordinator/coordinator.h
touch backend/coordinator/tablet_manager.cpp
touch backend/coordinator/tablet_manager.h

touch backend/storage/storage_node.cpp
touch backend/storage/storage_node.h
touch backend/storage/tablet.cpp
touch backend/storage/tablet.h

touch backend/account/account_server.cpp
touch backend/account/account_server.h
touch backend/account/user.cpp
touch backend/account/user.h

touch backend/email/smtp_server.cpp
touch backend/email/smtp_server.h
touch backend/email/pop3_server.cpp
touch backend/email/pop3_server.h
touch backend/email/email.cpp
touch backend/email/email.h

touch backend/storage_service/storage_server.cpp
touch backend/storage_service/storage_server.h
touch backend/storage_service/file.cpp
touch backend/storage_service/file.h

touch backend/admin/admin_server.cpp
touch backend/admin/admin_server.h
touch backend/admin/dashboard.cpp
touch backend/admin/dashboard.h

touch backend/network/tcp_server.cpp
touch backend/network/tcp_server.h
touch backend/network/tcp_client.cpp
touch backend/network/tcp_client.h

touch backend/server_main.cpp
touch backend/CMakeLists.txt

# 创建前端文件结构
mkdir -p frontend
mkdir -p frontend/js
mkdir -p frontend/css

touch frontend/index.html
touch frontend/login.html
touch frontend/home.html
touch frontend/admin.html
touch frontend/register.html
touch frontend/change_password.html

touch frontend/js/login.js
touch frontend/js/home.js
touch frontend/js/admin.js
touch frontend/js/tcp_client.js
touch frontend/js/utils.js
touch frontend/js/register.js
touch frontend/js/change_password.js
touch frontend/js/inbox.js
touch frontend/js/compose.js
touch frontend/js/files.js

touch frontend/css/styles.css
touch frontend/css/admin.css
touch frontend/css/home.css