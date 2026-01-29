#ifndef LIBBOORUDL_TAGS_H
#define LIBBOORUDL_TAGS_H

#include "boorudl/enums.h"

#include <initializer_list>
#include <set>
#include <string>
#include <string_view>
#include <utility>

namespace boorudl {
    class tags : public std::set<std::string> {
    private:
        std::string_view m_current_rating;
        std::string_view m_current_user;

    public:
        tags() = default;

        explicit tags(const std::string& tags);

        tags(std::initializer_list<std::string> tags);

        bool has_tag(const std::string& tag) const;

        tags& add_tag(const std::string& tag);

        tags& remove_tag(const std::string& tag);

        tags& add_tag_blacklist(const std::string& tag);

        tags& remove_tag_blacklist(const std::string& tag);

        tags& add_sort(sort::type type, bool ascending = true);

        tags& remove_sort(sort::type type, bool ascending = true);

        tags& add_rating(rating::type type, bool blacklisted = false);

        tags& remove_rating(rating::type type, bool blacklisted = false);

        tags& add_from_user(const std::string& user);

        tags& remove_from_user(const std::string& user);

        tags& add_blacklist_from_user(const std::string& user);

        tags& remove_blacklist_from_user(const std::string& user);

        tags& add_or(std::initializer_list<std::string> tags);

        tags& remove_or(std::initializer_list<std::string> tags);

        tags& remove_ai_slop();

        std::string to_string(char separator = '+') const;

        explicit operator std::string() const;

        static bool is_blacklist_tag(std::string_view tag);

        static std::string make_from_user_tag(const std::string& username);

        static std::string make_or_tag(std::initializer_list<std::string> tags);

    private:
        std::pair<iterator, bool> insert_tag(const std::string& tag, bool blacklisted);

        size_type erase_tag(const std::string& tag, bool blacklisted);
    };
} // boorudl

#endif //LIBBOORUDL_TAGS_H
