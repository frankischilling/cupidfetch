document.addEventListener("DOMContentLoaded", function () {
    const closeButton = document.querySelector('.closexd');
    const quoteInputContainer = document.getElementById('quoteInputContainer');
    const quoteInputForm = document.getElementById('quoteInputForm');
    const settingsLink = document.getElementById("settingsLink");
    const settingsPanel = document.getElementById("settingsPanel");
    const darkModeToggle = document.getElementById("darkModeToggle");
    const watchHistoryToggle = document.getElementById("watchHistoryToggle"); // Add this line

    let formVisible = true;

    if (closeButton && quoteInputContainer) {
        closeButton.addEventListener('click', function () {
            if (formVisible) {
                quoteInputContainer.style.height = '50px';
                quoteInputForm.style.display = 'none';
                formVisible = false;
            } else {
                quoteInputContainer.style.height = '';
                quoteInputForm.style.display = 'block';
                formVisible = true;
            }
        });
    }

    function applyDarkModeStyles() {
        const body = document.body;
        const videoListItems = document.querySelectorAll('#videoList li');
        const videoListLinks = document.querySelectorAll('#videoList a');
        const currentVideoLink = document.querySelector('#currentVideo a');
        const titlesToDisplay = document.querySelectorAll('#currentVideo p');

        // Reset styles to default
        videoListItems.forEach(item => {
            item.style.color = '';
            item.style.backgroundColor = '';
        });

        videoListLinks.forEach(link => {
            link.style.color = '';
        });

        if (currentVideoLink) {
            currentVideoLink.style.color = '';
        }

        titlesToDisplay.forEach(title => {
            title.style.color = '';
        });

        // Apply dark mode styles if dark mode is active
        if (body.classList.contains('dark-mode')) {
            videoListItems.forEach(item => {
                item.style.color = '#fff';
                item.style.backgroundColor = '#333';
            });

            videoListLinks.forEach(link => {
                link.style.color = '#fff';
            });

            if (currentVideoLink) {
                currentVideoLink.style.color = '#fff';
            }

            titlesToDisplay.forEach(title => {
                title.style.color = '#fff';
            });
        }
    }

    darkModeToggle.addEventListener("change", function () {
        const isDarkMode = darkModeToggle.checked;
        document.body.classList.toggle("dark-mode", isDarkMode);
        localStorage.setItem("darkMode", isDarkMode.toString());
        applyDarkModeStyles();
    });

    const isDarkMode = localStorage.getItem("darkMode") === "true";
    darkModeToggle.checked = isDarkMode;
    document.body.classList.toggle("dark-mode", isDarkMode);

    settingsLink.addEventListener("click", function (event) {
        event.preventDefault();
        settingsPanel.classList.toggle("active");
        document.addEventListener("click", closeSettingsPanel);
    });

    function closeSettingsPanel(event) {
        if (!settingsPanel.contains(event.target) && event.target !== settingsLink) {
            settingsPanel.classList.remove("active");
            document.removeEventListener("click", closeSettingsPanel);
        }
    }

    applyDarkModeStyles();

    // Event listener for Watch History toggle
    watchHistoryToggle.addEventListener("change", function () {
        const keepWatchHistory = watchHistoryToggle.checked;
        localStorage.setItem("keepWatchHistory", keepWatchHistory.toString());

        // Handle watch history visibility
        handleWatchHistory(keepWatchHistory);
    });

    // Check if watch history preference is stored in localStorage
    const keepWatchHistory = localStorage.getItem("keepWatchHistory") !== "false";
    watchHistoryToggle.checked = keepWatchHistory;

    // Initial call to handle watch history based on user preference
    handleWatchHistory(keepWatchHistory);

    function handleWatchHistory(keepWatchHistory) {
        const watchHistorySection = document.getElementById("watchHistorySection");

        if (keepWatchHistory) {
            watchHistorySection.style.display = "block";
        } else {
            watchHistorySection.style.display = "none";
        }
    }
});
