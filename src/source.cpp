#include "boorudl/credentials.h"
#include "boorudl/source.h"

#include <optional>
#include <string>
#include <utility>

namespace boorudl {
    source::source(const std::string& base_url)
        : source{ base_url, {} } { }

    source::source(const std::string& base_url, credentials credentials)
        : source{ base_url, { }, credentials } { }

    source::source(const std::string& base_url, const std::string& api_url)
        : m_base_url{ base_url }
        , m_api_url{ api_url } { }

    source::source(const std::string& base_url, const std::string& api_url, credentials credentials)
        : m_base_url{ base_url }
        , m_api_url{ api_url }
        , m_credentials{ credentials } { }

    const std::string& source::get_base_url() const {
        return m_base_url;
    }

    const std::string& source::get_api_url() const {
        return !m_api_url.empty()
            ? m_api_url
            : m_base_url;
    }

    bool source::has_credentials() const {
        return m_credentials.has_value();
    }

    const source::credentials_type& source::get_credentials() const {
        return m_credentials;
    }

    bool source::get_force_id_fallback() const {
        return m_force_id_fallback;
    }

    source& source::set_force_id_fallback(bool force_id_fallback) {
        m_force_id_fallback = force_id_fallback;
        return *this;
    }

    std::optional<const std::string> source::get_credentials_query_string() const {
        if (!has_credentials())
            return std::nullopt;

        return m_credentials.value().to_query_string();
    }
} // boorudl