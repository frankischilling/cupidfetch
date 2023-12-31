<?php
header('Content-Type: application/json');

// Replace this with your actual data storage method (e.g., database, file, etc.).
// For this example, we assume that messages are stored in a JSON file.
$messages = json_decode(file_get_contents('messages.json'), true);

if ($messages === null) {
    $messages = [];
}

$response = ['messages' => $messages];

echo json_encode($response);
