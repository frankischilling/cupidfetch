$('#searchInput').on('input', function() {
    var videoList = $('#videoList');
    var videoItems = videoList.find('.video-item');
    var randomVideoButton = $('#randomVideoButton');
    var quoteText = $('#quoteText');

    // Implement a user-friendly search
    $('#searchInput').on('input', function() {
        var searchInput = $(this).val().toLowerCase();

        videoItems.each(function() {
            var videoItem = $(this);
            var videoName = videoItem.text().toLowerCase();

            if (videoName.includes(searchInput)) {
                videoItem.show();
            } else {
                videoItem.hide();
            }
        });
    });

    // Show all videos when the search input is cleared
    $('#searchInput').on('input', function() {
        if ($(this).val() === '') {
            videoItems.show();
        }
    });
});
