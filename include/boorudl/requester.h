#ifndef LIBBOORUDL_REQUESTER_H
#define LIBBOORUDL_REQUESTER_H

#include "boorudl/downloadable.h"
#include "boorudl/exporter.h"
#include "boorudl/ids.h"
#include "boorudl/page.h"
#include "boorudl/post.h"
#include "boorudl/source.h"
#include "boorudl/tags.h"
#include "boorudl/request.h"

#include <string>
#include <vector>

namespace boorudl {
    class requester {
    public:
        using count_type = unsigned long;

    private:
        ids m_ids;
        tags m_tags;
        const std::string m_api_url;
        const source& m_source;

    public:
        requester(ids ids, tags tags, const source& source);

        requester(const requester&) = default;

        requester(requester&&) noexcept = default;

        request make_request(int items_per_page, int total_pages, int starting_page = 0) const;

        request make_request(int items_per_page, int total_pages, int starting_page, exporter_ref exporter) const;

        count_type get_post_count() const;

        const ids& get_ids() const;

        const tags& get_tags() const;

        requester& set_ids(const ids& ids);

        requester& set_tags(const tags& tags);

    private:
        std::vector<page> get_tags_posts(int items_per_page, int total_pages, int starting_page, exporter_ref exporter) const;

        std::string get_api_url(const source& source) const;
    };
} // boorudl

#endif //LIBBOORUDL_REQUESTER_H
