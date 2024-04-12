import socket
import json
import time

# 后端服务器设置
HOST = "localhost"
PORTS = {
    "coordinator": 8000,
    "account": 8002,
    "email": 8003,
    "storage": 8004,
    "admin": 8005
}

def send_request(host, port, method, args):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        request = {"method": method, "args": args}
        s.sendall(json.dumps(request).encode())
        response = s.recv(1024)
        return json.loads(response)

def test_account_server():
    print("Testing AccountServer...")
    response = send_request(HOST, PORTS["account"], "GET_USER", ["testuser"])
    assert response["success"] == False, "GET_USER should fail for non-existent user"

    response = send_request(HOST, PORTS["account"], "CREATE_USER", ["testuser", "testpass", "test@example.com"])
    assert response["success"] == True, "CREATE_USER failed"

    response = send_request(HOST, PORTS["account"], "GET_USER", ["testuser"])
    assert response["success"] == True, "GET_USER failed"
    assert response["result"] == "testuser\ntestpass\ntest@example.com"

    print("AccountServer test passed!")

def test_email_server():
    print("Testing EmailServer...")
    response = send_request(HOST, PORTS["email"], "SEND_EMAIL", ["sender@example.com", "receiver@example.com", "Test Subject", "Test Body"])
    assert response["success"] == True, "SEND_EMAIL failed"

    # Test POP3 server (requires setting up a separate POP3 server)
    # ...

    print("EmailServer test passed!")

def test_storage_server():
    print("Testing StorageServer...")
    response = send_request(HOST, PORTS["storage"], "STORE_FILE", ["test.txt", "This is a test file."])
    assert response["success"] == True, "STORE_FILE failed"

    response = send_request(HOST, PORTS["storage"], "GET_FILE", ["test.txt"])
    assert response["success"] == True, "GET_FILE failed"
    assert response["result"] == "test.txt\nThis is a test file."

    response = send_request(HOST, PORTS["storage"], "DELETE_FILE", ["test.txt"])
    assert response["success"] == True, "DELETE_FILE failed"

    print("StorageServer test passed!")

def test_admin_server():
    print("Testing AdminServer...")
    response = send_request(HOST, PORTS["admin"], "GET_SYSTEM_STATUS")
    assert response["success"] == True, "GET_SYSTEM_STATUS failed"
    print(response["result"])

    response = send_request(HOST, PORTS["admin"], "CONTROL_SYSTEM", ["restart_coordinator"])
    assert response["success"] == True, "CONTROL_SYSTEM failed"

    print("AdminServer test passed!")

if __name__ == '__main__':
    test_account_server()
    test_email_server()
    test_storage_server()
    test_admin_server()
    print("All tests passed!")