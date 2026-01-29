#ifndef LIBBOORUDL_FAVORITES_H
#define LIBBOORUDL_FAVORITES_H

#include "boorudl/common.h"
#include "boorudl/downloadable.h"
#include "boorudl/exporter.h"
#include "boorudl/ids.h"
#include "boorudl/page.h"
#include "boorudl/source.h"

#include <string>

namespace boorudl {
    class favorites : public downloadable_type {
    public:
        static constexpr int all_pages{ 0 };
        static constexpr int first_page{ 0 };
        static constexpr int max_items_per_page{ 50 };

    private:
        const common::id_type m_user_id{};
        const std::string m_favorites_url;
        const exporter_ref m_exporter;
        const source& m_source;

    public:
        favorites(common::id_type user_id, const source& source);

        favorites(common::id_type user_id, const source& source, int total_pages, int starting_page, exporter_ref exporter);

        size_t count() const;

        common::id_type get_user_id() const;

    private:
        void get_favorites_pages(int total_pages, int starting_page);

        page get_page(int page);

    public:
        static std::string get_favorites_url(common::id_type user_id, const std::string& base_url);

    private:
        static ids get_favorites_page_ids(const std::string& page_html);
    };
} // boorudl

#endif //LIBBOORUDL_FAVORITES_H
