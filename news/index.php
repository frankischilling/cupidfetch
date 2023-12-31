<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="shortcut icon" href="../img/favicon.ico" type="image/x-icon">
    <link rel="stylesheet" type="text/css" href="styles.css">
    <link href="https://fonts.googleapis.com/css2?family=Pacifico&display=swap" rel="stylesheet">
    <meta name="description" content="Welcome to the Tony Chase Archive, where Tony, the government, and even satellites join in a cosmic dance of creativity and humor. Discover how this remarkable man sends satellites spinning with laughter. Dive into a world where we celebrate coolness and cosmic connections. It's a place where satellites are not just in the sky; they're the life of the party!">
    <title>The Tony Chase Archive - News</title>
</head>
<a id="titleLink" href="http://tonychasearchive.com"><h1>Tony Chase Archive</h1></a>
    <main>
        <h2>News About Tony and the Archive</h2>
        <div class="news-post">
            <p><strong>October 28, 2023 - About Tony Chase Updates</strong></p>
            <p>The <a href="https://www.youtube.com/@TonyChaseArchive">YouTube channel</a> has about 1.2k videos on it so far, leading us right into 2021. I have also set up dontations for server costs too (click the CashApp image).</p>
            <p>Stil uploading around 100-120 videos aday. Around 3.5k videos saved on the S3 object storage. I hope everyone is having a good October, I plan to keep archiving our friend until the end of time.</p>
        </div>
        <div class="news-post">
            <p><strong>October 18, 2023 - About Tony Chase Updates</strong></p>
            <p>Did a shit ton of updates for the About page stuff go check it <a href="/about/">out!!!</a>
            <p>Again uploading around 100 videos every until I get my cooldown. Also was able to grab tony's 2010 channel so I wil be uploading that after I get done with his main content I think. Let me know if there is anything I should add to the site. I am ony one person and I am still learning but I can always give it my best shot.</p>
        </div>
        <div class="news-post">
            <p><strong>October 15, 2023 - Dealing with YouTube Issues</strong></p>
            <p>While attempting to upload more Tony Chase videos to the <a href="https://www.youtube.com/@TonyChaseArchive">archive channel</a>, we encountered challenges related to YouTube's daily upload limit.</p>
            <p>I did some research and discovered that by consistently uploading content daily and hitting the limit, as Tony does, over an extended period, we will gradually gain the ability to upload more content each day.</p> 
        </div>
        <div class="news-post">
            <p><strong>October 15, 2023 - Video Count and Archive Percentage</strong></p>
            <p>Right now on the server we use, we have a drive that has 200GB. That is enough to store around 2000 videos of Tony's channel. The issue is Tony has around 4.6k videos, meaning we won't be able to store all of the videos on the server at once. That's why we have the YouTube channel. I will be uploading the older Tony content so the server has space for the newer videos. That is the main reason you will see the count and percentage change.</p>
            <div id="stats">
            <p>storage: </p>
                <?php
                    $directory = '/mnt/tony/TonyChaseArchive/Youtube/'; // Replace with the path to your directory

                    if (is_dir($directory)) {
                        $totalSpace = disk_total_space($directory);
                        $freeSpace = disk_free_space($directory);
                        $usedSpace = $totalSpace - $freeSpace;

                        echo "Total Space: " . formatBytes($totalSpace) . "<br>";
                        echo "Free Space: " . formatBytes($freeSpace) . "<br>";
                        echo "Used Space: " . formatBytes($usedSpace);
                    } else {
                        echo "Invalid directory path.";
                    }

                    function formatBytes($bytes, $precision = 2) {
                        $units = array('B', 'KB', 'MB', 'GB', 'TB');
                        $bytes = max($bytes, 0);
                        $pow = floor(($bytes ? log($bytes) : 0) / log(1024));
                        $pow = min($pow, count($units) - 1);
                        $bytes /= (1 << (10 * $pow));
                        return round($bytes, $precision) . ' ' . $units[$pow];
                    }
                    ?>
            </div>  
         </div>
        <div id="infoContainer">
                <a href="https://cash.app/$TonyChaseArchive">
                    <img id="cashappLogo" src="../img/cash.png" alt="Cash App Logo">
                </a>         
            <div id="discordAndAbout">
                <a id="discordLink" href="https://discord.gg/beW5rY5Qrj">> Join our Discord server</a>
                <br>
                <a id="aboutLink" href="/about/">> About Tony Chase</a>
                <br>
                <a id="homeLink" href="index.php">> Tony Chase Archive (Home)</a>
            </div>
        </div>
    </main>

    <footer>
        Website made with ❤️
    </footer>
</body>
</html>