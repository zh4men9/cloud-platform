import TCPClient from './tcp_client.js';
import { serializeFiles, deserializeFiles } from './utils.js';

const fileList = document.getElementById('file-list');
const fileUpload = document.getElementById('file-upload');
const uploadButton = document.getElementById('upload-button');
const downloadButton = document.getElementById('download-button');
const deleteButton = document.getElementById('delete-button');
const tcpClient = new TCPClient('backend', 8080);

async function loadFiles() {
    try {
        await tcpClient.connect();
        const response = await tcpClient.send('GET_FILES');
        if (response.success) {
            const files = deserializeFiles(response.result);
            fileList.innerHTML = '';
            files.forEach(file => {
                const li = document.createElement('li');
                li.textContent = file.name;
                li.addEventListener('click', () => downloadFile(file.name));
                fileList.appendChild(li);
            });
        } else {
            console.error('Failed to load files:', response.result);
        }
    } catch (error) {
        console.error('Error loading files:', error);
    }
}

async function uploadFile() {
    try {
        const file = fileUpload.files[0];
        await tcpClient.connect();
        await tcpClient.send(`STORE_FILE ${file.name} ${file.content}`);
        loadFiles();
    } catch (error) {
        console.error('Error uploading file:', error);
    }
}

async function downloadFile(fileName) {
    try {
        await tcpClient.connect();
        const response = await tcpClient.send(`GET_FILE ${fileName}`);
        if (response.success) {
            const file = deserializeFiles(response.result)[0];
            const downloadLink = document.createElement('a');
            downloadLink.href = `data:application/octet-stream;base64,${btoa(file.data)}`;
            downloadLink.download = file.name;
            document.body.appendChild(downloadLink);
            downloadLink.click();
            document.body.removeChild(downloadLink);
        } else {
            console.error('Failed to download file:', response.result);
        }
    } catch (error) {
        console.error('Error downloading file:', error);
    }
}

async function deleteFile(fileName) {
    try {
        await tcpClient.connect();
        const response = await tcpClient.send(`DELETE_FILE ${fileName}`);
        if (response.success) {
            loadFiles();
        } else {
            console.error('Failed to delete file:', response.result);
        }
    } catch (error) {
        console.error('Error deleting file:', error);
    }
}

uploadButton.addEventListener('click', uploadFile);
downloadButton.addEventListener('click', () => {
    const selectedFile = fileList.querySelector('li.selected');
    if (selectedFile) {
        downloadFile(selectedFile.textContent);
    } else {
        console.error('No file selected for download.');
    }
});
deleteButton.addEventListener('click', () => {
    const selectedFile = fileList.querySelector('li.selected');
    if (selectedFile) {
        deleteFile(selectedFile.textContent);
    } else {
        console.error('No file selected for deletion.');
    }
});

loadFiles();