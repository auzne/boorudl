#ifndef BOORUDL_COMMENT_H
#define BOORUDL_COMMENT_H

#include "boorudl/common.h"
#include "boorudl/source.h"

#include <pugixml.hpp>
#include <string>
#include <string_view>
#include <vector>

namespace boorudl {
    class comment {
    public:
        using collection = std::vector<comment>;

    private:
        std::string m_created_at;
        common::id_type m_post_id{};
        std::string m_body;
        std::string m_creator;
        common::id_type m_id{};
        common::id_type m_creator_id{};

    public:
        explicit comment(pugi::xml_node comment_xml);

        std::string_view get_created_at() const;

        common::id_type get_post_id() const;

        std::string_view get_body() const;

        std::string_view get_creator() const;

        common::id_type get_id() const;

        common::id_type get_creator_id() const;

        static collection get_comments(common::id_type post_id, const source& source);

        static std::string get_api_url(common::id_type post_id, const source& source);
    };

    using comments = comment::collection;
} // boorudl

#endif //BOORUDL_COMMENT_H
