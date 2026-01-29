#include "boorudl/credentials.h"
#include "boorudl/source.h"

#include <optional>
#include <string>
#include <utility>

namespace boorudl {
    source::source(const std::string& base_url)
        : m_base_url{ base_url } { }

    source::source(const std::string& base_url, credentials credentials)
        : m_base_url{ base_url }
        , m_credentials{ std::move(credentials) } { }

    const std::string& source::get_base_url() const {
        return m_base_url;
    }

    bool source::has_credentials() const {
        return m_credentials.has_value();
    }

    const source::credentials_type& source::get_credentials() const {
        return m_credentials;
    }

    std::optional<const std::string> source::get_credentials_query_string() const {
        if (!has_credentials())
            return std::nullopt;

        return m_credentials.value().to_query_string();
    }
} // boorudl