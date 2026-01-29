#ifndef LIBBOORUDL_EXPORTER_H
#define LIBBOORUDL_EXPORTER_H

#include "boorudl/page.h"
#include "boorudl/post.h"

#include <optional>
#include <functional>

namespace boorudl {
    /* this interface can be used to export posts and pages with a user implemented class
     * implementations can be passed to 'requester::make_request' and 'favorites::favorites'
     * the functions must not throw any exceptions
     */
    class i_exporter {
    public:
        // This function is called for every post fetched
        virtual void on_post(const post& post) const noexcept = 0;

        // This function is called when a page is done and has at least one post
        virtual void on_page(const page& page) const noexcept = 0;

        virtual ~i_exporter() = default;
    };

    using exporter_ref = std::optional<std::reference_wrapper<const i_exporter>>;
} // boorudl

#endif //LIBBOORUDL_EXPORTER_H
