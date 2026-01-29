#include "boorudl/credentials.h"

#include <string>

namespace boorudl {
    credentials::credentials(long user_id, const std::string& api_key)
        : m_user_id { user_id }
        , m_api_key { api_key } { }

    std::string credentials::to_query_string() const {
        return "&user_id=" + std::to_string(m_user_id) + "&api_key=" + m_api_key;
    }
} // boorudl