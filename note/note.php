<?php
header('Content-Type: application/json');

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $message = $_POST['message'];

    // Add code to store the message in a JSON file or database.
    // For simplicity, let's store it in a JSON file.

    $messages = json_decode(file_get_contents('messages.json'), true);
    $messages[] = $message;
    file_put_contents('messages.json', json_encode($messages));

    // Return a response to the client.
    echo json_encode(['message' => 'Message received and stored.']);
} else {
    http_response_code(405); // Method Not Allowed
    echo json_encode(['message' => 'Invalid request method.']);
}
