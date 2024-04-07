import TCPClient from './tcp_client.js';
import { serializeEmail } from './utils.js';

const recipientInput = document.getElementById('recipient');
const subjectInput = document.getElementById('subject');
const bodyTextarea = document.getElementById('body');
const sendButton = document.getElementById('send-button');
const tcpClient = new TCPClient('backend', 8080);

async function sendEmail() {
    try {
        const username = localStorage.getItem('username');
        const recipient = recipientInput.value;
        const subject = subjectInput.value;
        const body = bodyTextarea.value;
        const email = { from: username, to: recipient, subject, body };

        await tcpClient.connect();
        await tcpClient.send(`SEND_EMAIL ${serializeEmail(email)}`);

        recipientInput.value = '';
        subjectInput.value = '';
        bodyTextarea.value = '';
        alert('Email sent successfully!');
    } catch (error) {
        console.error('Error sending email:', error);
        alert('Failed to send email. Please try again later.');
    }
}

sendButton.addEventListener('click', sendEmail);