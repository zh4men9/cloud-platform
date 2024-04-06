import { parseResponse } from './utils.js';

class TCPClient {
    constructor(host, port) {
        this.host = host;
        this.port = port;
        this.socket = null;
    }

    async connect() {
        this.socket = new WebSocket(`ws://${this.host}:${this.port}`);
        return new Promise((resolve, reject) => {
            this.socket.onopen = () => resolve();
            this.socket.onerror = (error) => reject(error);
        });
    }

    async send(data) {
        return new Promise((resolve, reject) => {
            this.socket.onmessage = (event) => {
                const response = parseResponse(event.data);
                resolve(response);
            };
            this.socket.onerror = (error) => reject(error);
            this.socket.send(data);
        });
    }
}

export default TCPClient;