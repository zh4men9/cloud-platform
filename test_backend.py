import socket
import json

HOST = "localhost"
PORT = 8000

def send_request(request_str):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(request_str.encode())
        response_data = s.recv(4096)
        response = response_data.decode()
        print("Response:", response)

# Test GET_ACCOUNT
request_str = "method:GET_ACCOUNT\ntestuser"
send_request(request_str)

# Test CREATE_ACCOUNT
request_str = "method:CREATE_ACCOUNT\ntestuser\ntestpassword\ntest@example.com"
send_request(request_str)

# Test SEND_EMAIL
request_str = "method:SEND_EMAIL\nfrom@example.com\nto@example.com\nTest Subject\nThis is the email body."
send_request(request_str)

# Test GET_FILE
request_str = "method:GET_FILE\ntest_file.txt"
send_request(request_str)

# Test STORE_FILE
request_str = "method:STORE_FILE\ntest_file.txt\nThis is the content of the file."
send_request(request_str)

# Test DELETE_FILE
request_str = "method:DELETE_FILE\ntest_file.txt"
send_request(request_str)

# Test GET_SYSTEM_STATUS
request_str = "method:GET_SYSTEM_STATUS"
send_request(request_str)

# Test CONTROL_SYSTEM
request_str = "method:CONTROL_SYSTEM\nrestart"
send_request(request_str)