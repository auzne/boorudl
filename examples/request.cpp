#include <boorudl/boorudl.h>
#include <iostream>
#include <filesystem>

int main() {
    // To create a source (where you will be downloading from)
    // You must pass the base website url with trailing slash '/'
    // Then, optionally, the user credentials, this vary from the website
    // There is no validation of any data passed
    boorudl::source source{ "https://safebooru.org/" };

    // With the source set you can begin to make post requests
    // Using boorudl::ids you can specify post ids to get them individually
    boorudl::ids ids{ 3372739 };
    
    // Using boorudl::tags you can specify post tags to get posts that have these tags
    boorudl::tags tags{ "xenoblade_chronicles_(series)", "official_art" };

    // boorudl::tags also offers other methods as alias for more specific tags
    // For example you can add a safe rating and block posts with generative AI tags
    // There also other methods to add sorting or backlisting tags
    tags
        .add_rating(boorudl::rating::type::safe)
        .remove_ai_slop();

    // After defining the ids or tags you can create a requester
    // boorudl::requester can also receive an exporter, which will be covered in another example
    boorudl::requester requester{ ids, tags, source };

    // From the requester you can get the total post count of the tags
    std::cout << "Post count: " << requester.get_post_count() << '\n';

    // The next step is requesting the posts
    // This will make the api calls to fetch the posts
    // You need to specify the items per page (max 1000)
    // how many pages will be downloaded
    // and optionally, the starting page (starts at 0)
    boorudl::request request{ requester.make_request(10, 1) };

    // After the request is done it'll have all the data stored and ready to download
    // You can pass some options to the downloader, but that will not be covered here
    boorudl::downloader downloader{ request };

    // Base download path
    std::filesystem::path download_path{ std::filesystem::temp_directory_path()
        / "booru"
        / "request" };

    // From the donwloader, we can download the posts in two ways
    // 1. Sequential: will download one file at a time, better for large files
    downloader.download(download_path);

    // 2. Batch download: will download N files at a time, specified by the second argument
    downloader.batch_download(download_path, 10);

    return 0;
}
