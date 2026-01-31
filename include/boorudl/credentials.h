#ifndef BOORUDL_CREDENTIALS_H
#define BOORUDL_CREDENTIALS_H

#include <optional>
#include <string>

namespace boorudl {
    class credentials {
    public:
        using credentials_opt = std::optional<credentials>;

    private:
        const long m_user_id{};
        const std::string m_api_key;

    public:
        credentials(long user_id, const std::string& api_key);

        credentials(const credentials&) = default;

        credentials(credentials&&) noexcept = default;

        std::string to_query_string() const;
    };
}

#endif // BOORUDL_CREDENTIALS_H
