#ifndef LIBBOORUDL_DOWNLOADER_H
#define LIBBOORUDL_DOWNLOADER_H

#include "boorudl/batches.h"
#include "boorudl/downloadable.h"
#include "boorudl/favorites.h"
#include "boorudl/options.h"
#include "boorudl/page.h"
#include "boorudl/request.h"

#include <filesystem>

namespace boorudl {
    class downloader {
    public:
        using path_type = std::filesystem::path;
        using batch_type = batches::batch_type;

        enum class download_status {
            no_posts_to_download,
            failed_to_create_directories,
            permission_denied,
            successful,
        };

    private:
        const downloadable_type& m_to_download;
        const options m_options{};

    public:
        explicit downloader(const downloadable_type& to_download, options options = {});

        download_status download(const path_type& download_path) const;

        download_status batch_download(const path_type& download_path, int batch_size) const;

    private:
        void download_page(const page& page, const path_type& path, int index) const;

        void download_batch(const batch_type& batch, const path_type& path, int index) const;

        path_type get_post_parent_directory(const post& post, const path_type& base_path, int page) const;

        static download_status create_directories(const path_type& download_path);
    };
} // boorudl

#endif //LIBBOORUDL_DOWNLOADER_H
