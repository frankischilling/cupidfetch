<?php

$tonyplayerDir = '../tonyplayer'; // adjust the path accordingly
$songList = [];

// Read the directory
$files = scandir($tonyplayerDir);

// Filter out unwanted files (e.g., ".", "..")
$files = array_diff($files, ['.', '..']);

// Create a list of songs with URLs and titles
foreach ($files as $file) {
    $url = "/tonyplayer/" . rawurlencode($file); // adjust the path accordingly
    $title = pathinfo($file, PATHINFO_FILENAME);
    $songList[] = ['url' => $url, 'title' => $title];
}

// Send the JSON response
header('Content-Type: application/json');
echo json_encode($songList);
