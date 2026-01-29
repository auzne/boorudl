#include "boorudl/constants.h"
#include "boorudl/curl_handler.h"
#include "boorudl/ids.h"
#include "boorudl/request.h"
#include "boorudl/requester.h"
#include "boorudl/tags.h"

#include <curl/curl.h>
#include <optional>
#include <pugixml.hpp>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace boorudl {
    requester::requester(ids ids, tags tags, const source& source)
        : m_ids{ std::move(ids) }
        , m_tags{ std::move(tags) }
        , m_api_url{ get_api_url(source) }
        , m_source{ source } { }

    request requester::make_request(int items_per_page, int total_pages, int starting_page) const {
        return make_request(items_per_page, total_pages, starting_page, std::nullopt);
    }

    request requester::make_request(int items_per_page, int total_pages, int starting_page, exporter_ref exporter) const {
        auto ids_posts{ !m_ids.empty()
            ? m_ids.to_page(m_source, exporter)
            : page{} };

        auto tags_posts{ !m_tags.empty()
            ? get_tags_posts(items_per_page, total_pages, starting_page, exporter)
            : std::vector<page>{} };

        return { ids_posts, tags_posts };
    }

    requester::count_type requester::get_post_count() const {
        curl::easy handle;
        std::stringstream stream;

        const std::string post_count_url{ m_api_url + "&limit=0" };

        CURLcode res{ handle
            .set_to_stream(stream, post_count_url)
            .perform() };

        if (res != CURLE_OK)
            return 0;

        stream.ignore(52);
        count_type count{};
        stream >> count;

        return count;
    }

    const ids& requester::get_ids() const {
        return m_ids;
    }

    const tags& requester::get_tags() const {
        return m_tags;
    }

    requester& requester::set_ids(const ids& ids) {
        m_ids = ids;

        return *this;
    }

    requester& requester::set_tags(const boorudl::tags& tags) {
        m_tags = tags;

        return *this;
    }

    std::vector<page> requester::get_tags_posts(int items_per_page, int total_pages, int starting_page, exporter_ref exporter) const {
        count_type post_count{ get_post_count() };

        /* Nothing to download */
        if (post_count == 0)
            return {};

        /* Insufficient items to download */
        if (post_count <= (items_per_page * static_cast<count_type>(starting_page)))
            return {};

        int available_pages{ total_pages };
        /* Has fewer items than requested, so avoid downloading more pages by decreasing @available_pages */
        if (post_count < (items_per_page * static_cast<count_type>(total_pages + starting_page)))
            available_pages = static_cast<int>(((post_count - items_per_page * starting_page) / items_per_page) + 1);

        curl::easy handle;
        std::stringstream stream;

        std::vector<page> result;
        result.reserve(available_pages);
        const std::string base_url{ m_api_url + "&limit=" + std::to_string(items_per_page) + "&pid=" };

        pugi::xml_document xml;
        for (int page_index = 0; page_index < available_pages; ++page_index) {
            int current_page{ starting_page + page_index };
            const std::string page_url{ base_url + std::to_string(current_page) };

            CURLcode res{ handle
                .set_to_stream(stream, page_url)
                .perform() };

            if (res != CURLE_OK) {
                // TODO: log fail
                continue;
            }

            auto xml_res{ xml.load(stream) };
            if (!xml_res)
                throw std::runtime_error{ xml_res.description() };

            result.emplace_back(xml.first_child());

            if (exporter) {
                const auto& page{ result.back() };
                if (!page.empty()) {
                    const auto& ref{ exporter->get() };
                    
                    for (const auto& post : page)
                        ref.on_post(post);
    
                    ref.on_page(page);
                }
            }
        }
        return result;
    }

    std::string requester::get_api_url(const source& source) const {
        const std::string url{ source.get_base_url()
            + std::string{ constants::posts_path }
            + "&tags="
            + m_tags.to_string() };

        return source.has_credentials()
            ? url + source.get_credentials().value().to_query_string()
            : url;
    }
} // boorudl