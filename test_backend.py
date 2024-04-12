import socket
import json
import time

# 后端服务器设置
HOST = "localhost"
PORT = 8000

def send_request(method, args):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        request = {"method": method, "args": args}
        s.sendall(json.dumps(request).encode())
        response = s.recv(4096)
        return json.loads(response)

def test_account_server():
    print("Testing AccountServer...")
    response = send_request("GET_ACCOUNT", ["testuser"])
    assert response["success"] == False, "GET_ACCOUNT should fail for non-existent user"

    response = send_request("CREATE_ACCOUNT", ["testuser", "testpass", "test@example.com"])
    assert response["success"] == True, "CREATE_ACCOUNT failed"

    response = send_request("GET_ACCOUNT", ["testuser"])
    assert response["success"] == True, "GET_ACCOUNT failed"
    assert response["result"] == "testuser\ntestpass\ntest@example.com"

    print("AccountServer test passed!")

def test_email_server():
    print("Testing EmailServer...")
    response = send_request("SEND_EMAIL", ["sender@example.com", "receiver@example.com", "Test Subject", "Test Body"])
    assert response["success"] == True, "SEND_EMAIL failed"

    print("EmailServer test passed!")

def test_storage_server():
    print("Testing StorageServer...")
    response = send_request("STORE_FILE", ["test.txt", "This is a test file."])
    assert response["success"] == True, "STORE_FILE failed"

    response = send_request("GET_FILE", ["test.txt"])
    assert response["success"] == True, "GET_FILE failed"
    assert response["result"] == "test.txt\nThis is a test file."

    response = send_request("DELETE_FILE", ["test.txt"])
    assert response["success"] == True, "DELETE_FILE failed"

    print("StorageServer test passed!")

def test_admin_server():
    print("Testing AdminServer...")
    response = send_request("GET_SYSTEM_STATUS", [])
    assert response["success"] == True, "GET_SYSTEM_STATUS failed"
    print(response["result"])

    response = send_request("CONTROL_SYSTEM", ["restart_coordinator"])
    assert response["success"] == True, "CONTROL_SYSTEM failed"

    print("AdminServer test passed!")

if __name__ == "__main__":
    test_account_server()
    test_email_server()
    test_storage_server()
    test_admin_server()
    print("All tests passed!")