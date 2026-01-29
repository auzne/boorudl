#include "boorudl/common.h"
#include "boorudl/constants.h"
#include "boorudl/curl_handler.h"
#include "boorudl/enums.h"
#include "boorudl/post.h"

#include <pugixml.hpp>
#include <sstream>
#include <string>
#include <string_view>

namespace boorudl {
    post::post(common::id_type id, const source& source)
        : post{ id, source.get_base_url(), source.get_credentials() } { }

    post::post(common::id_type id, const std::string& base_url, const credentials_type& credentials)
        : post{ get_xml_by_id(id, base_url, credentials) } { }

    post::post(pugi::xml_node post_xml)
        : m_preview_url{ post_xml.attribute("preview_url").as_string() }
        , m_sample_url{ post_xml.attribute("sample_url").as_string() }
        , m_file_url{ post_xml.attribute("file_url").as_string() }
        , m_directory{ post_xml.attribute("directory").as_llong() }
        , m_md5{ post_xml.attribute("md5").as_string() }
        , m_width{ post_xml.attribute("width").as_llong() }
        , m_height{ post_xml.attribute("height").as_llong() }
        , m_id{ post_xml.attribute("id").as_ullong() }
        , m_image{ post_xml.attribute("image").as_string() }
        , m_change{ post_xml.attribute("change").as_ullong() }
        , m_owner{ post_xml.attribute("owner").as_string() }
        , m_parent_id{ post_xml.attribute("parent_id").as_ullong() }
        , m_creator_id{ post_xml.attribute("creator_id").as_ullong() }
        , m_created_at{ post_xml.attribute("created_at").as_string() }
        , m_rating{ rating::parse(post_xml.attribute("rating").as_string()) }
        , m_sample{ post_xml.attribute("sample").as_bool() }
        , m_sample_height{ post_xml.attribute("sample_height").as_llong() }
        , m_sample_width{ post_xml.attribute("sample_width").as_llong() }
        , m_score{ post_xml.attribute("score").as_llong() }
        , m_tags(post_xml.attribute("tags").as_string()) /* must be direct initialization */
        , m_status{ post_xml.attribute("status").as_string() }
        , m_source{ post_xml.attribute("source").as_string() }
        , m_has_children{ post_xml.attribute("has_children").as_bool() }
        , m_has_notes{ post_xml.attribute("has_notes").as_bool() }
        , m_has_comments{ post_xml.attribute("has_comments").as_bool() }
        , m_file_extension{ m_file_url.substr(m_file_url.find_last_of('.')) } { }

    std::string_view post::get_file_url() const {
        return m_file_url;
    }

    common::id_type post::get_id() const {
        return m_id;
    }

    common::id_type post::get_creator_id() const {
        return m_creator_id;
    }

    rating::type post::get_rating() const {
        return m_rating;
    }

    const tags& post::get_tags() const {
        return m_tags;
    }

    std::string_view post::get_source() const {
        return m_source;
    }

    std::string_view post::get_file_extension() const {
        return m_file_extension;
    }

    std::string post::get_filename() const {
        return std::to_string(m_id) + m_file_extension;
    }

    bool post::has_tag(const std::string& tag) const {
        return m_tags.has_tag(tag);
    }

    std::string post::get_api_url(common::id_type id, const source& source) {
        return get_api_url(id, source.get_base_url(), source.get_credentials());
    }

    std::string post::get_api_url(common::id_type id, const std::string& base_url) {
        const std::string post_base_url{  base_url + std::string{ constants::posts_path } + "&id=" };
        return post_base_url + std::to_string(id);
    }

    std::string post::get_api_url(common::id_type id, const std::string& base_url, const std::string& credentials) {
        const std::string post_base_url{  base_url + std::string{ constants::posts_path } + "&id=" };
        return post_base_url + std::to_string(id) + credentials;
    }

    std::string post::get_api_url(common::id_type id, const std::string& base_url, const credentials_type& credentials) {
        return credentials.has_value()
            ? get_api_url(id, base_url, credentials.value().to_query_string())
            : get_api_url(id, base_url);
    }

    pugi::xml_node post::get_xml_by_id(common::id_type id, const std::string& base_url, const credentials_type& credentials) {
        curl::easy handle;
        std::stringstream stream;

        CURLcode res{ handle
            .set_to_stream(stream, get_api_url(id, base_url, credentials))
            .perform() };

        if (res != CURLE_OK) {
            throw std::runtime_error{ curl_easy_strerror(res) };
        }

        pugi::xml_document xml;
        auto xml_res{ xml.load(stream) };
        if (!xml_res)
            throw std::runtime_error{ xml_res.description() };

        auto posts_node{ xml.child("posts") };
        if (posts_node.attribute("count").as_int() == 0)
            throw std::invalid_argument{ "id" };

        return posts_node.child("post");
    }
} // boorudl