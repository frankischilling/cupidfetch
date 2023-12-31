document.addEventListener('DOMContentLoaded', () => {
    const messageForm = document.getElementById('message-form');
    const messageInput = document.getElementById('message-input');
    const messageContainer = document.getElementById('message-container');

    messageForm.addEventListener('submit', async (e) => {
        e.preventDefault();
        const message = messageInput.value;

        // Send the message to the PHP script on the server
        try {
            const response = await fetch('note.php', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: new URLSearchParams({ message: message }).toString(),
            });

            if (response.ok) {
                // If the PHP script successfully processes the message, display it on the page.
                const data = await response.json();
                displayMessage(data.message);
                messageInput.value = '';
            } else {
                console.error('Failed to submit message');
            }
        } catch (error) {
            console.error('An error occurred while sending the message:', error);
        }
    });

    function displayMessage(message) {
        const messageElement = document.createElement('div');
        messageElement.textContent = message;
        messageContainer.appendChild(messageElement);
    }

    // Add code to display existing user messages
    displayExistingMessages();

    function displayExistingMessages() {
        // Fetch existing messages from your server and display them
        fetch('get-message.php')
            .then(response => response.json())
            .then(data => {
                data.messages.forEach(message => {
                    displayMessage(message);
                });
            })
            .catch(error => {
                console.error('Failed to fetch existing messages:', error);
            });
    }
});
