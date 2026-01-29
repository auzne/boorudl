#include "boorudl/downloadable.h"
#include "boorudl/page.h"
#include "boorudl/request.h"

#include <utility>
#include <vector>

namespace boorudl {
    request::request(page ids_posts, downloadable_type tags_posts)
        : std::vector<page>{ merge_ids_tags(std::move(ids_posts), std::move(tags_posts)) } { }

    downloadable_type request::merge_ids_tags(page&& ids_posts, downloadable_type&& tags_posts) {
        tags_posts.insert(tags_posts.begin(), ids_posts);

        return tags_posts;
    }
} // boorudl