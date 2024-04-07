import TCPClient from './tcp_client.js';
import { parseResponse } from './utils.js';

const form = document.getElementById('register-form');
const messageElement = document.getElementById('register-message');
const tcpClient = new TCPClient('backend', 8080);

async function register(username, email, password) {
    await tcpClient.connect();
    const response = await tcpClient.send(`REGISTER ${username} ${email} ${password}`);
    return response;
}

form.addEventListener('submit', async (event) => {
    event.preventDefault();

    const username = document.getElementById('username').value;
    const email = document.getElementById('email').value;
    const password = document.getElementById('password').value;

    try {
        const response = await register(username, email, password);
        if (response.success) {
            messageElement.textContent = 'Registration successful. Please log in.';
            window.location.href = 'login.html';
        } else {
            messageElement.textContent = response.result;
        }
    } catch (error) {
        messageElement.textContent = 'An error occurred. Please try again later.';
    }
});