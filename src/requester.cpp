#include "boorudl/constants.h"
#include "boorudl/curl_handler.h"
#include "boorudl/ids.h"
#include "boorudl/requester.h"
#include "boorudl/tags.h"

#include <cmath>
#include <curl/curl.h>
#include <optional>
#include <pugixml.hpp>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace boorudl {
    requester::requester(ids ids, const source& source)
        : requester{ ids, {}, source } { }

    requester::requester(tags tags, const source& source)
        : requester{ {}, tags, source } { }

    requester::requester(ids ids, tags tags, const source& source)
        : m_ids{ std::move(ids) }
        , m_tags{ std::move(tags) }
        , m_source{ source }
        , m_ids_api_url{ get_api_url(m_ids.to_tags(), source) }
        , m_tags_api_url{ get_api_url(m_tags, source) } { }

    downloadable_type requester::make_request(int items_per_page, int total_pages, int starting_page) const {
        return make_request(items_per_page, total_pages, starting_page, std::nullopt);
    }

    downloadable_type requester::make_request(int items_per_page, int total_pages, int starting_page, exporter_ref exporter) const {
        auto ids_posts{ get_ids_posts(items_per_page, exporter) };

        auto tags_posts{ get_tags_posts(m_tags, m_tags_api_url, items_per_page, total_pages, starting_page, exporter) };

        // move tags posts to the ids vector
        if (!tags_posts.empty()) {
            ids_posts.insert(
                ids_posts.end(),
                std::make_move_iterator(tags_posts.begin()),
                std::make_move_iterator(tags_posts.end()));
        }

        return ids_posts;
    }

    downloadable_type requester::get_ids_posts(int items_per_page) const {
        return get_ids_posts(items_per_page, std::nullopt);
    }

    downloadable_type requester::get_ids_posts(int items_per_page, exporter_ref exporter) const {
        if (m_ids.empty())
            return {};

        // ids are passed through query string, not a good idea to have that many
        if (items_per_page > 100)
            items_per_page = 100;

        const int size{ static_cast<int>(m_ids.size()) };
        const int total_pages{ static_cast<int>(std::ceil(size / static_cast<double>(items_per_page))) }; 

        return get_tags_posts(m_ids.to_tags(), m_ids_api_url, items_per_page, total_pages, 0, exporter, true);
    }

    requester::count_type requester::get_post_count() const {
        if (m_tags.empty())
            return 0;

        curl::easy handle;
        std::stringstream stream;

        const std::string post_count_url{ m_tags_api_url + "&limit=0" };

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

    downloadable_type requester::get_tags_posts(
        const tags& tags, const std::string& api_url, int items_per_page,
        int total_pages, int starting_page, exporter_ref exporter, bool isIds
    ) const {
        if (tags.empty())
            return {};

        int available_pages{ total_pages };

        if (!isIds) {
            count_type post_count{ get_post_count() };
    
            /* Nothing to download */
            if (post_count == 0)
                return {};
    
            /* Insufficient items to download */
            if (post_count <= (items_per_page * static_cast<count_type>(starting_page)))
                return {};

            /* Has fewer items than requested, so avoid downloading more pages by decreasing @available_pages */
            if (post_count < (items_per_page * static_cast<count_type>(total_pages + starting_page)))
                available_pages = static_cast<int>(((post_count - items_per_page * starting_page) / items_per_page) + 1);
        }

        curl::easy handle;
        std::stringstream stream;

        std::vector<page> result;
        result.reserve(available_pages);
        const std::string base_url{ api_url + "&limit=" + std::to_string(items_per_page) + "&pid=" };

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

    std::string requester::get_api_url(const tags& tags, const source& source) {
        if (tags.empty())
            return {};

        const std::string url{ source.get_base_url()
            + std::string{ constants::posts_path }
            + "&tags="
            + tags.to_string() };

        return source.has_credentials()
            ? url + source.get_credentials().value().to_query_string()
            : url;
    }
} // boorudl