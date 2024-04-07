import TCPClient from './tcp_client.js';
import { deserializeEmail, serializeEmail } from './utils.js';

const emailList = document.getElementById('email-list');
const emailContentElement = document.getElementById('email-content');
const replyButton = document.getElementById('reply-button');
const forwardButton = document.getElementById('forward-button');
const deleteButton = document.getElementById('delete-button');
const tcpClient = new TCPClient('backend', 8080);

async function loadEmails(username) {
    try {
        await tcpClient.connect();
        const response = await tcpClient.send(`GET_EMAILS ${username}`);
        if (response.success) {
            const emails = response.result.split('\n').map(deserializeEmail);
            emailList.innerHTML = '';
            emails.forEach(email => {
                const li = document.createElement('li');
                li.textContent = `From: ${email.from}, Subject: ${email.subject}`;
                li.addEventListener('click', () => showEmailContent(email));
                emailList.appendChild(li);
            });
        } else {
            console.error('Failed to load emails:', response.result);
        }
    } catch (error) {
        console.error('Error loading emails:', error);
    }
}

function showEmailContent(email) {
    emailContentElement.innerHTML = `
        <h3>${email.subject}</h3>
        <p>From: ${email.from}</p>
        <p>${email.body}</p>
    `;
}

replyButton.addEventListener('click', async () => {
    const username = localStorage.getItem('username');
    const subject = `Re: ${emailContentElement.querySelector('h3').textContent}`;
    const body = prompt('Enter your reply:');
    const email = { from: username, to: emailContentElement.querySelector('p:first-of-type').textContent.split(': ')[1], subject, body };
    await tcpClient.connect();
    await tcpClient.send(`SEND_EMAIL ${serializeEmail(email)}`);
    loadEmails(username);
});

forwardButton.addEventListener('click', async () => {
    const username = localStorage.getItem('username');
    const subject = `Fwd: ${emailContentElement.querySelector('h3').textContent}`;
    const body = prompt('Enter your message:');
    const email = { from: username, to: prompt('Enter recipient email:'), subject, body };
    await tcpClient.connect();
    await tcpClient.send(`SEND_EMAIL ${serializeEmail(email)}`);
    loadEmails(username);
});

deleteButton.addEventListener('click', async () => {
    const username = localStorage.getItem('username');
    const subject = emailContentElement.querySelector('h3').textContent;
    await tcpClient.connect();
    await tcpClient.send(`DELETE_EMAIL ${username} ${subject}`);
    loadEmails(username);
    emailContentElement.innerHTML = '';
});

loadEmails(localStorage.getItem('username'));