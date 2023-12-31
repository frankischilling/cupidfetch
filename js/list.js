document.addEventListener("DOMContentLoaded", function () {
    const closeButton = document.querySelector('.closexd');
    const quoteInputContainer = document.getElementById('quoteInputContainer');
    const quoteInputForm = document.getElementById('quoteInputForm');

    let formVisible = true;

    if (closeButton && quoteInputContainer) {
        closeButton.addEventListener('click', function () {
            if (formVisible) {
                quoteInputContainer.style.height = '50px'; // Adjust the height as needed
                quoteInputForm.style.display = 'none'; // Hide the form inputs
                formVisible = false;
            } else {
                quoteInputContainer.style.height = ''; // Reset the height
                quoteInputForm.style.display = 'block'; // Show the form inputs
                formVisible = true;
            }
        });
    }

    const randomVideoButton = document.getElementById('randomVideoButton');
    const newVideoButton = document.getElementById('newVideo'); // Reference to the "New Video" button
    const CrazyButton = document.getElementById('crazyButton'); // Reference to the "New Video" button
    const videoContainer = document.getElementById('randomVideoContainer');

    let currentVideoElement = null; // Store the reference to the current video element
    const playedVideoTitles = [];
    const currentVideoElementTitle = document.getElementById('currentVideo');

    function updateCurrentVideoTitle(videoTitle, videoURL) {
        const currentVideoElement = document.getElementById('currentVideo');

        if (videoTitle && videoURL) {
            currentVideoElement.innerHTML = `Current Video: <a href="${videoURL}" target="_blank">${videoTitle}</a>`;
        } else {
            currentVideoElement.innerHTML = 'Current Video: No video playing';
        }
    }

    function updateWatchHistoryUI() {
        const watchHistoryList = document.getElementById('watchHistoryList');
        const watchHistory = getWatchHistory();

        watchHistoryList.innerHTML = ''; // Clear existing history

        watchHistory.forEach(video => {
            const listItem = document.createElement('li');
            const link = document.createElement('a');

            link.href = video.url;
            link.textContent = video.title;
            link.target = '_blank';

            // Add click event to play the video in the current video player
            link.addEventListener('click', function (event) {
                event.preventDefault();
                playVideoInCurrentPlayer(video.url, video.title);
            });

            listItem.appendChild(link);
            watchHistoryList.appendChild(listItem);
        });
    }

    updateWatchHistoryUI();

    function updateVideoList() {
        const xmlUrl = 'https://tonyy.us-southeast-1.linodeobjects.com';
        videoList.innerHTML = '';
        let totalVideoCount = 0;

        function getVideoData(marker = '') {
            fetch(`${xmlUrl}?marker=${marker}`)
                .then(response => response.text())
                .then(data => {
                    const parser = new DOMParser();
                    const xmlDoc = parser.parseFromString(data, "text/xml");
                    const keyElements = xmlDoc.querySelectorAll('Key');

                    keyElements.forEach(keyElement => {
                        const keyText = keyElement.textContent;
                        if (keyText.endsWith('.mp4')) {
                            const li = document.createElement('li');
                            li.classList.add('video-item');
                            const a = document.createElement('a');
                            const titleAndDate = keyText.split('/').pop();

                            a.href = `https://tonyy.us-southeast-1.linodeobjects.com/${encodeURI(keyText)}`;
                            a.textContent = titleAndDate;
                            a.target = '_blank';

                            li.appendChild(a);
                            videoList.appendChild(li);
                            totalVideoCount++;
                        }
                    });

                    const isTruncated = xmlDoc.querySelector('IsTruncated').textContent;
                    if (isTruncated === 'true') {
                        const nextMarker = keyElements[keyElements.length - 1].textContent;
                        getVideoData(nextMarker);
                    } else {
                        document.getElementById('totalVideoCount').textContent = totalVideoCount;
                        createAndPlayRandomVideo(); // Play a random video after updating the list
                    }
                })
                .catch(error => {
                    console.error('Error fetching data:', error);
                });
        }

        getVideoData();
    }

    function openRandomVideo() {
        const videoItems = document.querySelectorAll('.video-item');
        if (videoItems.length === 0) {
            return;
        }

        const randomIndex = Math.floor(Math.random() * videoItems.length);
        const randomVideoURL = videoItems[randomIndex].querySelector('a').href;

        // Assuming getVideoTitle is a function that returns the title from the URL
        const videoTitle = getVideoTitle(randomVideoURL);

        // Now, you can use videoTitle in the addToHistory function
        addToHistory(videoTitle, randomVideoURL);

        window.open(randomVideoURL, '_blank');
    }

    function createAndPlayRandomVideo() {
        const videoItems = document.querySelectorAll('.video-item');
        if (videoItems.length === 0) {
            return;
        }

        const randomIndex = Math.floor(Math.random() * videoItems.length);
        const randomVideoURL = videoItems[randomIndex].querySelector('a').href;



        if (currentVideoElement) {
            videoContainer.removeChild(currentVideoElement);
        }

        // Update the video title when a new video is played
        const videoTitle = getVideoTitle(randomVideoURL);
        playedVideoTitles.push(videoTitle);
        updateCurrentVideoTitle(videoTitle, randomVideoURL);

        const videoElement = document.createElement('video');
        videoElement.controls = true;
        videoElement.autoplay = false;
        videoElement.src = randomVideoURL;
        videoElement.width = 800; // Change to your desired width in pixels
        videoElement.height = 450; // Change to your desired height in pixels


        currentVideoElement = videoElement;
        videoContainer.appendChild(videoElement);
    }

    newVideoButton.addEventListener('click', createAndPlayRandomVideo);
    function createAndPlayRandomVideoReal() {
        const videoItems = document.querySelectorAll('.video-item');
        if (videoItems.length === 0) {
            return;
        }

        const randomIndex = Math.floor(Math.random() * videoItems.length);
        const randomVideoURL = videoItems[randomIndex].querySelector('a').href;

        if (currentVideoElement) {
            videoContainer.removeChild(currentVideoElement);
        }

        // Update the video title when a new video is played
        const videoTitle = getVideoTitle(randomVideoURL);
        playedVideoTitles.push(videoTitle);
        updateCurrentVideoTitle(videoTitle, randomVideoURL);
        addToHistory(videoTitle, randomVideoURL);

        const videoElement = document.createElement('video');
        videoElement.controls = true;
        videoElement.autoplay = true;
        videoElement.src = randomVideoURL;
        videoElement.width = 640; // Change to your desired width in pixels
        videoElement.height = 360; // Change to your desired height in pixels

        currentVideoElement = videoElement;
        videoContainer.appendChild(videoElement);
    }

    // Define an array to keep track of played video elements
    const playedVideos = [];

    function Crazy() {
        const videoItems = document.querySelectorAll('.video-item');
        if (videoItems.length === 0) {
            return;
        }

        // Prompt the user for the number of random videos to play
        const videosToPlay = prompt("How many random videos do you want to play?");
        if (!videosToPlay || isNaN(videosToPlay) || videosToPlay <= 0) {
            alert("Please enter a valid number of videos to play.");
            return;
        }

        let playedCount = 0; // Initialize the count of played videos

        // Check if there's an existing video element and remove it
        if (currentVideoElement) {
            videoContainer.removeChild(currentVideoElement);
            currentVideoElement = null;
            currentVideoURL = null; // Clear the current video URL
        }

        const titlesToDisplay = []; // Create an array to store the titles

        while (playedCount < videosToPlay && videoItems.length > 0) {
            const randomIndex = Math.floor(Math.random() * videoItems.length);
            const randomVideoURL = videoItems[randomIndex].querySelector('a').href;

            if (playedVideos.includes(randomVideoURL)) {
                // Skip videos that have already been played
                continue;
            }

            console.log(`Playing video: ${randomVideoURL}`); // Log the random video URL

            // Create a video element
            const videoElement = document.createElement('video');
            videoElement.controls = true;
            videoElement.autoplay = true;
            videoElement.width = 640; // Set the desired width in pixels
            videoElement.height = 360; // Set the desired height in pixels
            videoElement.src = randomVideoURL;

            videoContainer.appendChild(videoElement);

            // Store the current video URL
            const videoTitle = getVideoTitle(randomVideoURL);
            titlesToDisplay.push(videoTitle);
            playedVideos.push(randomVideoURL); // Add the video to the list of played videos
            playedCount++; // Increment the count for each played video

            // Remove the played video from the list to prevent it from being played again
            videoItems[randomIndex].remove();
        }

        // Update the current video titles and dates after playing the videos
        if (titlesToDisplay.length > 0) {
            updateCurrentVideoURLCrazy(titlesToDisplay); // Separate titles and display them
        }

        // Log the number of videos played
        console.log(`Played ${playedCount} videos.`);
    }

    function updateCurrentVideoURLCrazy(titlesToDisplay) {
        const currentVideoElement = document.getElementById('currentVideo');
        if (titlesToDisplay.length > 0) {
            const titlesHTML = titlesToDisplay.map(title => `<p>${title}</p>`).join('');
            currentVideoElement.innerHTML = '<p>Current Videos:</p>' + titlesHTML;
        } else {
            currentVideoElement.innerHTML = '<p>Current Video: No videos playing</p>';
        }
    }


    function getVideoTitle(videoURL) {
        const url = new URL(videoURL);
        const pathSegments = url.pathname.split('/');
        const fileName = decodeURI(pathSegments[pathSegments.length - 1]);
        return fileName.split('.mp4')[0];
    }

    CrazyButton.addEventListener('click', Crazy);
    randomVideoButton.addEventListener('click', openRandomVideo);
    newVideoButton.addEventListener('click', createAndPlayRandomVideoReal);

    const toggleButton = document.getElementById('toggleVideoListButton');

    toggleButton.addEventListener('click', function () {
        if (videoList.style.display === 'none' || videoList.style.display === '') {
            videoList.style.display = 'block';
        } else {
            videoList.style.display = 'none';
        }
    });

    updateVideoList();

});

function addToHistory(videoTitle, videoURL) {
    const history = JSON.parse(localStorage.getItem('watchHistory')) || [];
    const videoInfo = { title: videoTitle, url: videoURL };

    // Check if the video is already in the history to avoid duplicates
    const isVideoInHistory = history.some(item => item.url === videoURL);

    if (!isVideoInHistory) {
        history.push(videoInfo);
        localStorage.setItem('watchHistory', JSON.stringify(history));
    }
}

// Function to retrieve the watch history
function getWatchHistory() {
    return JSON.parse(localStorage.getItem('watchHistory')) || [];
}