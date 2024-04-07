export function parseResponse(responseStr) {
    const parts = responseStr.split(' ');
    const success = parts[0] === 'SUCCESS';
    const result = parts.slice(1).join(' ');
    return { success, result };
}

export function serializeEmail(email) {
    return `${email.from},${email.subject},${email.body}`;
}

export function deserializeEmail(emailStr) {
    const parts = emailStr.split(',');
    return {
        from: parts[0],
        subject: parts[1],
        body: parts[2]
    };
}

export function serializeFiles(files) {
    return JSON.stringify(files);
}

export function deserializeFiles(filesStr) {
    return JSON.parse(filesStr);
}