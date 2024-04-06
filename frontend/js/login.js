import TCPClient from './tcp_client.js';
import { parseResponse } from './utils.js';

const form = document.getElementById('login-form');
const messageElement = document.getElementById('login-message');
const tcpClient = new TCPClient('backend', 8080);

async function login(username, password) {
    await tcpClient.connect();
    const response = await tcpClient.send(`LOGIN ${username} ${password}`);
    return response;
}

form.addEventListener('submit', async (event) => {
    event.preventDefault();

    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;

    try {
        const response = await login(username, password);
        if (response.success) {
            localStorage.setItem('username', username);
            window.location.href = 'home.html';
        } else {
            messageElement.textContent = response.result;
        }
    } catch (error) {
        messageElement.textContent = 'An error occurred. Please try again later.';
    }
});