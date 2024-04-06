import TCPClient from './tcp_client.js';
import { parseResponse } from './utils.js';

const statusElement = document.getElementById('system-status');
const controlInput = document.getElementById('control-input');
const controlButton = document.getElementById('control-button');
const controlResult = document.getElementById('control-result');
const tcpClient = new TCPClient('backend', 8080);

async function getSystemStatus() {
    try {
        await tcpClient.connect();
        const response = await tcpClient.send('GET_STATUS');
        statusElement.textContent = response.result;
    } catch (error) {
        console.error('Error getting system status:', error);
    }
}

async function controlSystem(command) {
    try {
        await tcpClient.connect();
        const response = await tcpClient.send(`CONTROL ${command}`);
        controlResult.textContent = response.result;
    } catch (error) {
        console.error('Error controlling the system:', error);
    }
}

controlButton.addEventListener('click', async () => {
    const command = controlInput.value;
    await controlSystem(command);
});

getSystemStatus();