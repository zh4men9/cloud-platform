import TCPClient from './tcp_client.js';
import { parseResponse } from './utils.js';

const form = document.getElementById('change-password-form');
const messageElement = document.getElementById('change-password-message');
const tcpClient = new TCPClient('backend', 8080);

async function changePassword(currentPassword, newPassword) {
    await tcpClient.connect();
    const username = localStorage.getItem('username');
    const response = await tcpClient.send(`CHANGE_PASSWORD ${username} ${currentPassword} ${newPassword}`);
    return response;
}

form.addEventListener('submit', async (event) => {
    event.preventDefault();

    const currentPassword = document.getElementById('current-password').value;
    const newPassword = document.getElementById('new-password').value;

    try {
        const response = await changePassword(currentPassword, newPassword);
        if (response.success) {
            messageElement.textContent = 'Password changed successfully.';
        } else {
            messageElement.textContent = response.result;
        }
    } catch (error) {
        messageElement.textContent = 'An error occurred. Please try again later.';
    }
});