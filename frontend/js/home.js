import TCPClient from './tcp_client.js';
import { deserializeEmail, serializeFiles, deserializeFiles } from './utils.js';

const usernameElement = document.getElementById('username');
const emailList = document.getElementById('email-list');
const fileList = document.getElementById('file-list');
const fileUpload = document.getElementById('file-upload');
const uploadButton = document.getElementById('upload-button');
const tcpClient = new TCPClient('backend', 8080);

async function loadUserData() {
    try {
        await tcpClient.connect();
        const username = localStorage.getItem('username');
        usernameElement.textContent = username;

        const emailResponse = await tcpClient.send(`GET_EMAILS ${username}`);
        if (emailResponse.success) {
            const emails = emailResponse.result.split('\n').map(deserializeEmail);
            emails.forEach(email => {
                const li = document.createElement('li');
                li.textContent = `From: ${email.from}, Subject: ${email.subject}`;
                emailList.appendChild(li);
            });
        }

        const fileResponse = await tcpClient.send('GET_FILES');
        if (fileResponse.success) {
            const files = deserializeFiles(fileResponse.result);
            files.forEach(file => {
                const li = document.createElement('li');
                li.textContent = file.name;
                fileList.appendChild(li);
            });
        }
    } catch (error) {
        console.error('Error loading user data:', error);
    }
}

uploadButton.addEventListener('click', async () => {
    try {
        const file = fileUpload.files[0];
        await tcpClient.connect();
        await tcpClient.send(`STORE_FILE ${file.name} ${file.content}`);
        fileList.innerHTML = '';
        loadUserData();
    } catch (error) {
        console.error('Error uploading file:', error);
    }
});

loadUserData();