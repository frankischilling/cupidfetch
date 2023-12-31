document.addEventListener("DOMContentLoaded", function() {
    const updateBanner = document.getElementById("updateBanner");
    const closeUpdateButton = document.getElementById("closeUpdateButton");

    // Check if there's an update to display (you can add this condition)
    const hasUpdate = true;

    if (hasUpdate) {
        // Show the update banner
        updateBanner.style.display = "block";
    }

    closeUpdateButton.addEventListener("click", function() {
        // Hide the update banner
        updateBanner.style.display = "none";
    });
});
