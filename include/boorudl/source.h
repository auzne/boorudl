#ifndef LIBBOORUDL_SOURCE_H
#define LIBBOORUDL_SOURCE_H

#include "boorudl/credentials.h"

#include <filesystem>
#include <optional>
#include <string>

namespace boorudl {
    class source {
    public:
        using path_type = std::filesystem::path;
        using credentials_type = credentials::credentials_opt;

    private:
        const std::string m_base_url;
        const path_type m_download_path;
        const credentials::credentials_opt m_credentials;

    public:
        source(const std::string& base_url, path_type base_path);

        source(const std::string& base_url, path_type base_path, credentials credentials);
        
        source(const source&) = default;

        source(source&&) noexcept = default;

        const std::string& get_base_url() const;

        const path_type& get_download_path() const;

        bool has_credentials() const;

        const credentials_type& get_credentials() const;

        std::optional<const std::string> get_credentials_query_string() const;
    };
} // boorudl

#endif //LIBBOORUDL_SOURCE_H
