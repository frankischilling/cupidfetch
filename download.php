<?php
// Ensure the user is authorized to download
// Add any necessary checks for security

// Receive video paths from the JavaScript
$json = file_get_contents('php://input');
$data = json_decode($json);

if ($data && isset($data->videoPaths) && is_array($data->videoPaths)) {
    $videoPaths = $data->videoPaths;

    // Create a temporary directory to store the videos
    $tempDir = 'temp';
    if (!is_dir($tempDir)) {
        mkdir($tempDir);
    }

    // Construct full video URLs based on the base URL
    $baseURL = 'https://tonyy.us-southeast-1.linodeobjects.com';
    $videoURLs = array_map(function ($videoPath) use ($baseURL) {
        return $baseURL . '/' . $videoPath;
    }, $videoPaths);

    // Download each video and save it in the temporary directory
    foreach ($videoURLs as $index => $videoURL) {
        $videoContent = file_get_contents($videoURL);
        file_put_contents("$tempDir/video$index.mp4", $videoContent);
    }

    // Create a zip file
    $zip = new ZipArchive();
    $zipFileName = 'videos.zip';
    if ($zip->open($zipFileName, ZipArchive::CREATE) === TRUE) {
        // Add each video file to the zip archive
        $videoFiles = glob("$tempDir/*.mp4");
        foreach ($videoFiles as $videoFile) {
            $zip->addFile($videoFile, basename($videoFile));
        }

        // Close the zip archive
        $zip->close();

        // Send the zip file to the user for download
        header('Content-Type: application/zip');
        header("Content-Disposition: attachment; filename=$zipFileName");
        header('Content-Length: ' . filesize($zipFileName));
        readfile($zipFileName);

        // Clean up temporary files
        foreach ($videoFiles as $videoFile) {
            unlink($videoFile);
        }
        rmdir($tempDir);

        // Delete the zip file after download
        unlink($zipFileName);

        exit;
    } else {
        // Handle zip archive creation failure
        echo 'Failed to create the zip archive.';
    }
} else {
    // Handle invalid input from JavaScript
    echo 'Invalid input.';
}
