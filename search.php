<?php
$xmlUrl = 'https://tonyy.us-southeast-1.linodeobjects.com';
$xmlData = file_get_contents($xmlUrl);
$xml = simplexml_load_string($xmlData);

// Process the XML data, extract video links, and generate an HTML list
$htmlList = '<ul>';
foreach ($xml->Contents as $content) {
    $key = (string)$content->Key;
    if (pathinfo($key, PATHINFO_EXTENSION) === 'mp4') {
        $htmlList .= '<li><a href="' . $key . '" target="_blank">' . basename($key) . '</a></li>';
    }
}
$htmlList .= '</ul>';

echo $htmlList;
?>