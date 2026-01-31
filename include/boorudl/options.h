#ifndef BOORUDL_OPTIONS_H
#define BOORUDL_OPTIONS_H

#include "boorudl/common.h"
#include "boorudl/enums.h"
#include "boorudl/post.h"

#include <functional>

namespace boorudl {
    class options {
    public:
        enum class on_duplicate {
            skip,
            overwrite,
        };

        enum class group_type {
            none,
            creator,
            rating,
            file_extension,
            page,
        };

    private:
        on_duplicate m_duplicate{ on_duplicate::skip };
        group_type m_group_by{ group_type::none };
        common::filter_type m_filter{};

    public:
        options() = default;

        options(on_duplicate duplicate, group_type group_by, common::filter_type filter);

        options& set_on_duplicate(on_duplicate duplicate);

        options& set_group_type(group_type group_by);

        options& set_filter_type(common::filter_type filter);

        friend class downloader;
    };
} // boorudl

#endif //BOORUDL_OPTIONS_H
