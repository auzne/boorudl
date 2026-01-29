#ifndef LIBBOORUDL_POST_H
#define LIBBOORUDL_POST_H

#include "boorudl/common.h"
#include "boorudl/credentials.h"
#include "boorudl/enums.h"
#include "boorudl/source.h"
#include "boorudl/tags.h"

#include <pugixml.hpp>
#include <string>
#include <string_view>
#include <vector>

namespace boorudl {
    class post {
    public:
        using credentials_type = credentials::credentials_opt;

    private:
        const std::string m_preview_url;
        const std::string m_sample_url;
        const std::string m_file_url;
        const long long m_directory{};
        const std::string m_md5;
        const long long m_width{};
        const long long m_height{};
        const common::id_type m_id{};
        const std::string m_image;
        const common::id_type m_change{};
        const std::string m_owner;
        const common::id_type m_parent_id{};
        const common::id_type m_creator_id{};
        const std::string m_created_at;
        const rating::type m_rating{};
        const bool m_sample{};
        const long long m_sample_height{};
        const long long m_sample_width{};
        const long long m_score{};
        const tags m_tags;
        const std::string m_status;
        const std::string m_source;
        const bool m_has_children{};
        const bool m_has_notes{};
        const bool m_has_comments{};
        const std::string m_file_extension;

    public:
        post(common::id_type id, const source& source);

        post(common::id_type id, const std::string& base_url, const credentials_type& credentials);

        explicit post(pugi::xml_node post_xml);

        post(const post&) = default;

        post(post&&) noexcept = default;

        std::string_view get_file_url() const;

        common::id_type get_id() const;

        common::id_type get_creator_id() const;

        rating::type get_rating() const;

        const tags& get_tags() const;

        std::string_view get_source() const;

        std::string_view get_file_extension() const;

        std::string get_filename() const;

        bool has_tag(const std::string& tag) const;

        static std::string get_api_url(common::id_type id, const source& source);
        
        static std::string get_api_url(common::id_type id, const std::string& base_url);

        static std::string get_api_url(common::id_type id, const std::string& base_url, const std::string& credentials);

        static std::string get_api_url(common::id_type id, const std::string& base_url, const credentials_type& credentials);

    private:
        static pugi::xml_node get_xml_by_id(common::id_type id, const std::string& base_url, const credentials_type& credentials);
    };
} // boorudl

#endif //LIBBOORUDL_POST_H
