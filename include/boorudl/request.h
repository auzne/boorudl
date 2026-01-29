#ifndef LIBBOORUDL_REQUEST_H
#define LIBBOORUDL_REQUEST_H

#include "boorudl/downloadable.h"
#include "boorudl/page.h"

#include <vector>

namespace boorudl {
    class request : public downloadable_type {
    public:
        request(page ids_posts, downloadable_type tags_posts);

        request(const request&) = default;

        request(request&&) noexcept = default;

    private:
        static downloadable_type merge_ids_tags(page&& ids_posts, downloadable_type&& tags_posts);
    };
} // boorudl

#endif //LIBBOORUDL_REQUEST_H
