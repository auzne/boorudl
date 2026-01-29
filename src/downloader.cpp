#include "boorudl/batches.h"
#include "boorudl/curl_handler.h"
#include "boorudl/downloader.h"
#include "boorudl/options.h"
#include "boorudl/page.h"

#include <curl/curl.h>
#include <exception>
#include <filesystem>
#include <fstream>
#include <utility>
#include <vector>

namespace boorudl {
    downloader::downloader(const downloadable_type& to_download, options options)
        : m_to_download{ to_download }
        , m_options{ std::move(options) } { }

    downloader::download_status downloader::download(const path_type& download_path) const {
        if (m_to_download.empty())
            return download_status::no_posts_to_download;

        /* create directories */
        auto path_status{ create_directories(download_path) };
        if (path_status != download_status::successful)
            return path_status;

        for (downloadable_type::size_type index{ 0 }; index < m_to_download.size(); ++index)
            download_page(m_to_download[index], download_path, index);

        return download_status::successful;
    }

    downloader::download_status downloader::batch_download(const path_type& download_path, int batch_size) const {
        if (m_to_download.empty())
            return download_status::no_posts_to_download;

        /* create directories */
        auto path_status{ create_directories(download_path) };
        if (path_status != download_status::successful)
            return path_status;

        batches batches{ m_to_download, batch_size, m_options.m_filter };
        for (batches::size_type index{ 0 }; index < batches.size(); ++index)
            download_batch(batches[index], download_path, index);

        return download_status::successful;
    }

    void downloader::download_page(const page& page, const path_type& path, int index) const {
        for (const auto& post : page) {
            /* m_filter is a valid function and returns false */
            if (m_options.m_filter && !m_options.m_filter(post))
                continue;

            // parent directory of the file
            auto parent_dir{ get_post_parent_directory(post, path, index + 1) };
                        
            // create directory when grouping and skip download if failed
            if (m_options.m_group_by != options::group_type::none &&
                create_directories(parent_dir) != download_status::successful)
                continue;

            auto file_path{ parent_dir / post.get_filename() };

            /* skip already downloaded posts */
            if (m_options.m_duplicate == options::on_duplicate::skip &&
                std::filesystem::exists(file_path))
                continue;

            curl::easy handle;
            std::ofstream file{ file_path };

            const std::string download_url{ post.get_file_url() };
            CURLcode res{ handle
                .set_to_stream(file, download_url)
                .perform() };

            if (res != CURLE_OK) {
                // TODO: log fail
                continue;
            }
        }
    }

    void downloader::download_batch(const batch_type& batch, const path_type& path, int index) const {
        if (batch.empty())
            return;

        std::vector<curl::easy> handles;
        std::vector<std::ofstream> files;
        handles.reserve(batch.size());
        files.reserve(batch.size());

        for (const auto& post : batch) {
            // parent directory of the file
            auto parent_dir{ get_post_parent_directory(post, path, index + 1) };
            
            // create directory when grouping and skip download if failed
            if (m_options.m_group_by != options::group_type::none &&
                create_directories(parent_dir) != download_status::successful)
                continue;

            // create file path
            auto file_path{ parent_dir / post.get_filename() };

            /* skip already downloaded posts */
            if (m_options.m_duplicate == options::on_duplicate::skip &&
                std::filesystem::exists(file_path))
                continue;

            /* Populate vectors */
            handles.emplace_back();
            files.emplace_back(std::move(file_path));

            const std::string download_url{ post.get_file_url() };
            handles.back().set_to_stream(files.back(), download_url);
        }

        curl::multi multi;
        multi.resolve(handles);
    }

    downloader::path_type downloader::get_post_parent_directory(const post& post, const path_type& base_path, int page) const {
        static const std::string page_prefix{ "page_" };
        switch (m_options.m_group_by) {
            case options::group_type::none:
                // no grouping
                return base_path;
            case options::group_type::creator: {
                // use the creator id
                const auto creator_id{ std::to_string(post.get_creator_id()) };
                return base_path / creator_id;
            }
            case options::group_type::rating: {
                // use the rating
                const auto rating_dir{ rating::to_string(post.get_rating()) };
                return base_path / rating_dir;
            }
            case options::group_type::file_extension:
                // use the extension without the dot
                return base_path / post.get_file_extension().substr(1);
            case options::group_type::page: {
                // use the post page from the request or the batch index
                const auto page_directory{ page_prefix + std::to_string(page) };
                return base_path / page_directory;
            }
            default:
                throw std::logic_error("Not implemented");
        }
    }

    downloader::download_status downloader::create_directories(const path_type& download_path) {
        namespace fs = std::filesystem;
        try {
            path_type path{ fs::weakly_canonical(download_path) };
            if ((!fs::exists(path) && !fs::create_directories(path)) || !fs::is_directory(path)) {
                return download_status::failed_to_create_directories;
            }
        } catch (const std::filesystem::filesystem_error&) {
            return download_status::permission_denied;
        }

        return download_status::successful;
    }
} // boorudl