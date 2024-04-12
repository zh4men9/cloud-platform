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

# Test CREATE_ACCOUNT
request_str = "method:CREATE_ACCOUNT|||testuser|||testpassword|||test@example.com"
send_request(request_str)

# Test GET_ACCOUNT
request_str = "method:GET_ACCOUNT|||testuser"
send_request(request_str)

# Test GET_FILE
request_str = "method:GET_FILE|||test_file.txt"
send_request(request_str)

# Test STORE_FILE
request_str = "method:STORE_FILE|||test_file.txt|||This is the content of the file."
send_request(request_str)

# Test DELETE_FILE
request_str = "method:DELETE_FILE|||test_file.txt"
send_request(request_str)

# Test GET_SYSTEM_STATUS
request_str = "method:GET_SYSTEM_STATUS"
send_request(request_str)

# Test CONTROL_SYSTEM
request_str = "method:CONTROL_SYSTEM|||restart"
send_request(request_str)

# Test SEND_EMAIL
request_str = "method:SEND_EMAIL|||from@example.com|||to@example.com|||Test Subject|||This is the email body."
send_request(request_str)