#ifndef BOORUDL_REQUESTER_H
#define BOORUDL_REQUESTER_H

#include "boorudl/downloadable.h"
#include "boorudl/exporter.h"
#include "boorudl/ids.h"
#include "boorudl/page.h"
#include "boorudl/post.h"
#include "boorudl/source.h"
#include "boorudl/tags.h"

#include <string>
#include <vector>

namespace boorudl {
    class requester {
    public:
        using count_type = unsigned long;

    private:
        ids m_ids;
        tags m_tags;
        const source& m_source;
        const std::string m_ids_api_url;
        const std::string m_tags_api_url;

    public:
        requester(ids ids, const source& source);

        requester(tags tags, const source& source);

        requester(ids ids, tags tags, const source& source);

        requester(const requester&) = default;

        requester(requester&&) noexcept = default;

        downloadable_type make_request(int items_per_page, int total_pages, int starting_page = 0) const;

        downloadable_type make_request(int items_per_page, int total_pages, int starting_page, exporter_ref exporter) const;

        downloadable_type get_ids_posts(int items_per_page) const;

        downloadable_type get_ids_posts(int items_per_page, exporter_ref exporter) const;

        count_type get_post_count() const;

        const ids& get_ids() const;

        const tags& get_tags() const;

    private:
        downloadable_type get_tags_posts(
            const tags& tags, const std::string& api_url, int items_per_page,
            int total_pages, int starting_page, exporter_ref exporter, bool isIds = false
        ) const;

        static std::string get_api_url(const tags& tags, const source& source);
    };
} // boorudl

#endif //BOORUDL_REQUESTER_H
