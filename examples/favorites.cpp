#include <boorudl/boorudl.h>
#include <iostream>
#include <filesystem>

int main() {
    // Create a source for the webservice (more info at examples/requester.cpp)
    boorudl::source source{ "https://safebooru.org/" };

    // To fetch favorites from a user, you need only specify the user id and source
    // For youself, go to My Account then My profile (at least in safebooru)
    // On instance it'll beguin fetching the posts
    boorudl::favorites favorites{ 80465, source };

    // From the favorites you can get the total favorites count of the user
    std::cout << "Favorites count: " << favorites.count() << '\n';

    // With the instance you can create a downloader, and from here on out it'll be the same
    boorudl::downloader downloader{ favorites };

    // Create download path
    std::filesystem::path download_path{ std::filesystem::temp_directory_path()
        / "booru"
        / "favorites" };

    // Download in batches
    downloader.batch_download(download_path, 20);

    return 0;
}
