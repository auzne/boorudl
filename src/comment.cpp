#include "boorudl/comment.h"
#include "boorudl/common.h"
#include "boorudl/constants.h"
#include "boorudl/credentials.h"
#include "boorudl/curl_handler.h"
#include "boorudl/source.h"

#include <exception>
#include <pugixml.hpp>
#include <sstream>
#include <string>
#include <string_view>

namespace boorudl {
    comment::comment(pugi::xml_node comment_xml)
        : m_created_at{ comment_xml.attribute("created_at").as_string() }
        , m_post_id{ comment_xml.attribute("post_id").as_ullong() }
        , m_body{ comment_xml.attribute("body").as_string() }
        , m_creator{ comment_xml.attribute("creator").as_string() }
        , m_id{ comment_xml.attribute("id").as_ullong() }
        , m_creator_id{ comment_xml.attribute("creator_id").as_ullong() } { }

    std::string_view comment::get_created_at() const {
        return m_created_at;
    }

    common::id_type comment::get_post_id() const {
        return m_post_id;
    }

    std::string_view comment::get_body() const {
        return m_body;
    }

    std::string_view comment::get_creator() const {
        return m_creator;
    }

    common::id_type comment::get_id() const {
        return m_id;
    }

    common::id_type comment::get_creator_id() const {
        return m_creator_id;
    }

    comments comment::get_comments(common::id_type post_id, const source& source) {
        curl::easy handle;
        std::stringstream stream;

        CURLcode res{ handle
            .set_to_stream(stream, get_api_url(post_id, source))
            .perform() };

        if (res != CURLE_OK)
            throw std::runtime_error{ curl_easy_strerror(res) };

        pugi::xml_document xml;
        auto xml_res{ xml.load(stream) };
        if (!xml_res)
            throw std::runtime_error{ xml_res.description() };

        comments result;
        auto comments_node{ xml.child("comments") };

        for (const auto& comment : comments_node.children())
            result.emplace_back(comment);

        return result;
    }

    std::string comment::get_api_url(common::id_type post_id, const source& source) {
        const std::string url{ source.get_base_url()
            + std::string{ constants::comments_path }
            + std::to_string(post_id) };

        return source.has_credentials()
            ? url + source.get_credentials().value().to_query_string()
            : url;
    }
} // boorudl