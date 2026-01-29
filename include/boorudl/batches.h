#ifndef LIBBOORUDL_BATCHES_H
#define LIBBOORUDL_BATCHES_H

#include "boorudl/common.h"
#include "boorudl/downloadable.h"
#include "boorudl/page.h"
#include "boorudl/post.h"

#include <vector>

namespace boorudl {
    class batches : public std::vector<std::vector<post>> {
    public:
        using batch_type = std::vector<post>;

    private:
        const int m_batch_size{};
        common::filter_type m_filter;

    public:
        batches(const page& page, int batch_size, common::filter_type filter);

        batches(const downloadable_type& to_download, int batch_size, common::filter_type filter);

    private:
        void create_page_batches(const page& page);

        void create_filtered_batches(const page& page);

        void create_unfiltered_batches(const page& page);

        void optimize_batches();

        void move_batch(iterator to, iterator from);
    };
} // boorudl

#endif //LIBBOORUDL_BATCHES_H
