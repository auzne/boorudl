#include "boorudl/enums.h"
#include "boorudl/tags.h"

#include <initializer_list>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

// indicates a blacklisted tag
constexpr char blacklist_char{ '-' };

namespace boorudl {
    tags::tags(const std::string& tags) {
        std::stringstream stream{ tags };
        std::string value;

        while (stream >> value)
            insert(std::move(value));
    }

    tags::tags(std::initializer_list<std::string> tags)
        : std::set<std::string>{ tags } { }

    bool tags::has_tag(const std::string& tag) const {
#if __cplusplus >= 202002L
        return contains(tag);
#else
        return find(tag) != end();
#endif
    }

    boorudl::tags& tags::add_tag(const std::string& tag) {
        insert_tag(tag, false);
        return *this;
    }

    boorudl::tags& tags::remove_tag(const std::string& tag) {
        erase_tag(tag, false);
        return *this;
    }

    boorudl::tags& tags::add_tag_blacklist(const std::string& tag) {
        insert_tag(tag, true);
        return *this;
    }

    boorudl::tags& tags::remove_tag_blacklist(const std::string& tag) {
        erase_tag(tag, true);
        return *this;
    }

    boorudl::tags& tags::add_sort(sort::type type, bool ascending) {
        insert(sort::build_tag(type, ascending));
        return *this;
    }

    boorudl::tags& tags::remove_sort(sort::type type, bool ascending) {
        erase_tag(sort::build_tag(type, ascending), false);
        return *this;
    }

    boorudl::tags& tags::add_rating(rating::type type, bool blacklisted) {
        auto result{ insert_tag(rating::build_tag(type), blacklisted) };

        // Saves rating, making it not possible to add two ratings using add_rating, as it will cancel each other
        if (result.second) {
            if (!m_current_rating.empty())
                erase_tag(std::string{ m_current_rating }, false);

            m_current_rating = *result.first;
        }
        return *this;
    }

    boorudl::tags& tags::remove_rating(rating::type type, bool blacklisted) {
        auto tag{ rating::build_tag(type) };
        erase_tag(tag, blacklisted);

        if (m_current_rating == tag)
            m_current_rating = {};

        return *this;
    }

    boorudl::tags& tags::add_from_user(const std::string& user) {
        auto tag{ make_from_user_tag(user) };
        auto result{ insert_tag(tag, false) };

        // Saves user, making it not possible to add two users using add_from_user, as it will cancel each other
        if (result.second) {
            if (!m_current_user.empty())
                erase_tag(std::string{ m_current_user }, false);

            m_current_user = *result.first;
        }
        return *this;
    }

    boorudl::tags& tags::remove_from_user(const std::string& user) {
        auto tag{ make_from_user_tag(user) };
        erase_tag(tag, false);

        if (m_current_user == tag)
            m_current_user = {};

        return *this;
    }

    boorudl::tags& tags::add_blacklist_from_user(const std::string& user) {
        auto tag{ make_from_user_tag(user) };
        auto result{ insert_tag(tag, true) };
        return *this;
    }

    boorudl::tags& tags::remove_blacklist_from_user(const std::string& user) {
        auto tag{ make_from_user_tag(user) };
        erase_tag(tag, true);
        return *this;
    }

    boorudl::tags& tags::add_or(std::initializer_list<std::string> tags) {
        if (tags.size() == 0)
            throw std::invalid_argument{ "'tags' must have at least one item" };

        auto tag{ make_or_tag(tags) };
        insert_tag(tag, false);
        return *this;
    }

    boorudl::tags& tags::remove_or(std::initializer_list<std::string> tags) {
        if (tags.size() == 0)
            throw std::invalid_argument{ "'tags' must have at least one item" };

        auto tag{ make_or_tag(tags) };
        erase_tag(tag, false);
        return *this;
    }

    boorudl::tags& tags::remove_ai_slop() {
        // remove most popular ai tags
        insert_tag("ai_generated", true);
        insert_tag("ai_assisted", true);
        insert_tag("ai_hands", true);
        insert_tag("ai_art", true);
        insert_tag("ai-created", true);

        return *this;
    }

    std::string tags::to_string(char separator) const {
        std::string result;

        bool first{ true };
        for (std::string_view tag : *this) {
            if (!first) result += separator;
            else first = false;
            result += tag;
        }

        return result;
    }

    tags::operator std::string() const {
        return to_string(' ');
    }

    bool tags::is_blacklist_tag(std::string_view tag) {
        return tag[0] == blacklist_char;
    }

    std::string tags::make_from_user_tag(const std::string& username) {
        return "user:" + username;
    }

    std::string tags::make_or_tag(std::initializer_list<std::string> tags) {
        const auto size{ tags.size() };
        if (size == 0)
            return {};

        if (size == 1)
            return *tags.begin();

        std::ostringstream oss;
        oss << "(+";

        auto first{ tags.begin() };
        for (auto it{ first }; it != tags.end(); ++it) {
            if (it != first)
                oss << "+~+";

            oss << *it;
        }

        oss << "+)";
        return oss.str();
    }

    std::pair<tags::iterator, bool> tags::insert_tag(const std::string& tag, bool blacklisted) {
        return insert(blacklisted && !is_blacklist_tag(tag)
            ? blacklist_char + tag
            : tag);
    }

    tags::size_type tags::erase_tag(const std::string& tag, bool blacklisted) {
        return erase(blacklisted && !is_blacklist_tag(tag)
            ? blacklist_char + tag
            : tag);
    }
} // boorudl