#ifndef LIBBOORUDL_SOURCE_H
#define LIBBOORUDL_SOURCE_H

#include "boorudl/credentials.h"

#include <optional>
#include <string>

namespace boorudl {
    class source {
    public:
        using credentials_type = credentials::credentials_opt;

    private:
        const std::string m_base_url;
        const credentials::credentials_opt m_credentials;

    public:
        explicit source(const std::string& base_url);

        source(const std::string& base_url, credentials credentials);
        
        source(const source&) = default;

        source(source&&) noexcept = default;

        const std::string& get_base_url() const;

        bool has_credentials() const;

        const credentials_type& get_credentials() const;

        std::optional<const std::string> get_credentials_query_string() const;
    };
} // boorudl

#endif //LIBBOORUDL_SOURCE_H
