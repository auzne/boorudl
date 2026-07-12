#ifndef BOORUDL_SOURCE_H
#define BOORUDL_SOURCE_H

#include "boorudl/credentials.h"

#include <optional>
#include <string>

namespace boorudl {
    class source {
    public:
        using credentials_type = credentials::credentials_opt;
        static constexpr int no_limit{ 0 };

    private:
        const std::string m_base_url;
        const std::string m_api_url;
        const credentials::credentials_opt m_credentials;
        bool m_force_id_fallback{ false };
        int m_request_refresh_s{ no_limit };

    public:
        explicit source(const std::string& base_url);

        source(const std::string& base_url, credentials credentials);
        
        source(const std::string& base_url, const std::string& api_url);

        source(const std::string& base_url, const std::string& api_url, credentials credentials);

        source(const source&) = default;

        source(source&&) noexcept = default;

        const std::string& get_base_url() const;

        const std::string& get_api_url() const;

        bool has_credentials() const;

        const credentials_type& get_credentials() const;

        bool get_force_id_fallback() const;

        source& set_force_id_fallback(bool force_id_fallback);

        int get_request_refresh_s() const;

        source& set_request_refresh_s(int request_refresh_s);

        std::optional<const std::string> get_credentials_query_string() const;
    };
} // boorudl

#endif //BOORUDL_SOURCE_H
