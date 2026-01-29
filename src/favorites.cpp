#include "boorudl/constants.h"
#include "boorudl/curl_handler.h"
#include "boorudl/exporter.h"
#include "boorudl/favorites.h"
#include "boorudl/ids.h"
#include "boorudl/page.h"
#include "boorudl/source.h"

#include <curl/curl.h>
#include <regex>
#include <string>
#include <sstream>

namespace boorudl {
    favorites::favorites(common::id_type user_id, const source& source)
        : favorites(user_id, source, all_pages, first_page, std::nullopt) { }
    
    favorites::favorites(common::id_type user_id, const source& source, int total_pages, int starting_page, exporter_ref exporter)
        : m_user_id{ user_id }
        , m_favorites_url{ get_favorites_url(user_id, source.get_base_url()) }
        , m_exporter{ exporter }
        , m_source{ source } {
        get_favorites_pages(total_pages, starting_page);
    }

    size_t favorites::count() const {
        size_t total_posts{};

        for (const auto& page : *this) {
            total_posts += page.size();
        }

        return total_posts;
    }

    common::id_type favorites::get_user_id() const {
        return m_user_id;
    }

    void favorites::get_favorites_pages(int total_pages, int starting_page) {
        int current_page{ starting_page };
        
        /* loop until the last page */
        while (total_pages == all_pages || current_page < (total_pages + starting_page)) {
            push_back(get_page(current_page));
            auto current{ back() };

            /* is_missing_ids is true when there are fewer posts than expected, because some were deleted */
            bool is_missing_ids{ current.is_missing_ids() };

            /* when is_missing_ids is false and there are fewer items than @max_items_per_page, it's the last page */
            if (!is_missing_ids && current.size() < max_items_per_page)
                break;

            ++current_page;
        }
    }

    page favorites::get_page(int page) {
        curl::easy handle;
        std::stringstream stream;

        const std::string page_url{ m_favorites_url + std::to_string(page * max_items_per_page) };
        CURLcode res{ handle
            .set_to_stream(stream, page_url)
            .mimic_user_agent()
            .perform() };

        if (res != CURLE_OK) {
            // TODO: log fail
            return {};
        }

        return get_favorites_page_ids(stream.str())
            .to_page(m_source, m_exporter);
    }

    ids favorites::get_favorites_page_ids(const std::string& page_html) {
        static const std::regex id_finder{ "<a href=\"index\\.php\\?page=post&amp;s=view&amp;id=(\\d+)\"" };

        ids page_ids;
        for (std::sregex_iterator current{ page_html.begin(), page_html.end(), id_finder }, last;
             current != last; ++current) {
            page_ids.insert(std::stoul(current->str(1)));
        }
        return page_ids;
    }

    std::string favorites::get_favorites_url(common::id_type user_id, const std::string& base_url) {
        return base_url
            + std::string{ constants::favorites_path }
            + std::to_string(user_id)
            + "&pid=";
    }
} // boorudl