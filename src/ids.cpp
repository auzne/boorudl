#include "boorudl/common.h"
#include "boorudl/curl_handler.h"
#include "boorudl/exporter.h"
#include "boorudl/ids.h"
#include "boorudl/page.h"
#include "boorudl/post.h"

#include <initializer_list>
#include <pugixml.hpp>
#include <sstream>
#include <set>

namespace boorudl {
    ids::ids(std::initializer_list<common::id_type> ids)
        : std::set<common::id_type>{ ids } { }

    page ids::to_page(const source& source) const {
        return to_page(source, std::nullopt);
    }

    page ids::to_page(const source& source, exporter_ref exporter) const {
        if (this->empty())
            return {};

        std::vector<curl::easy> handles;
        std::vector<std::stringstream> streams;
        handles.reserve(size());
        streams.reserve(size());

        auto base_url{ source.get_base_url() };
        auto credentials_str{ source.get_credentials_query_string() };

        for (auto id : *this) {
            /* Populate vectors */
            handles.emplace_back();
            streams.emplace_back();

            const std::string download_url{ credentials_str.has_value()
                ? post::get_api_url(id, base_url, credentials_str.value())
                : post::get_api_url(id, base_url) };

            handles.back().set_to_stream(streams.back(), download_url);
        }

        curl::multi multi;
        multi.resolve(handles);

        page ids_page;
        ids_page.reserve(size());

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

        return ids_page;
    }
} // boorudl