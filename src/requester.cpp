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
        , m_tags_api_url{ get_api_url(m_tags, source) }
        , m_fallback_api_url{ get_fallback_api_url(source) } { }

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

        if (m_source.get_force_id_fallback()) {
            return get_ids_posts_fallback(items_per_page, exporter);
        }

        try {
            return get_tags_posts(
                m_ids.to_tags(), m_ids_api_url, items_per_page,
                total_pages, 0, exporter, true);
        } catch (std::runtime_error&) {
            // this error is thrown when the xml is invalid
            // happens when the api does not allow to fetch multiple ids as tags
            return get_ids_posts_fallback(items_per_page, exporter);
        }
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
        int total_pages, int starting_page, exporter_ref exporter, bool is_ids
    ) const {
        if (tags.empty())
            return {};

        int available_pages{ total_pages };

        if (!is_ids) {
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

    downloadable_type requester::get_ids_posts_fallback(int items_per_page, exporter_ref exporter) const {
        if (m_ids.empty())
            return {};

        const auto batches{ m_ids.to_batches(items_per_page) };
        
        downloadable_type result;
        result.reserve(batches.size());

        for (const auto& batch : batches) {
            std::vector<curl::easy> handles;
            std::vector<std::stringstream> streams;

            handles.reserve(items_per_page);
            streams.reserve(items_per_page);

            for (const auto& id : batch) {
                /* Populate vectors */
                handles.emplace_back();
                streams.emplace_back();
    
                const std::string download_url{ m_fallback_api_url + std::to_string(id) };
                handles.back()
                    .set_to_stream(streams.back(), download_url)
                    .set_redirect(CURLFOLLOW_FIRSTONLY);
            }

            curl::multi multi;
            multi.resolve(handles);
    
            page ids_page;
            ids_page.reserve(items_per_page);
    
            pugi::xml_document xml;
            for (auto& stream : streams) {
                auto xml_res{ xml.load(stream) };
                if (!xml_res) {
                    // TODO: log on fail
                    continue;
                }
    
                auto posts_node{ xml.child("posts") };
                // TODO: log on fail, this can happen when id does not exist or the post has been deleted
                if (posts_node.attribute("count").as_int() == 0) {
                    ids_page.set_is_missing_ids(true);
                    continue;
                }
    
                ids_page.emplace_back(posts_node.child("post"));
    
                if (exporter)
                    exporter->get().on_post(ids_page.back());
            }
    
            if (exporter)
                exporter->get().on_page(ids_page);

            result.push_back(ids_page);
        }

        return result;
    }

    std::string requester::get_api_url(const tags& tags, const source& source) {
        if (tags.empty())
            return {};

        const std::string url{ source.get_api_url()
            + std::string{ constants::posts_path }
            + "&tags="
            + tags.to_string() };

        return source.has_credentials()
            ? url + source.get_credentials().value().to_query_string()
            : url;
    }

    std::string requester::get_fallback_api_url(const source& source) {
        std::string url{ source.get_api_url() + std::string{ constants::posts_path } };

        if (source.has_credentials())
            url += source.get_credentials().value().to_query_string();

        return url + "&id=";
    }
} // boorudl