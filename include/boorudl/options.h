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

        static constexpr int no_file_size_limit{ 0 };

    private:
        on_duplicate m_duplicate{ on_duplicate::skip };
        group_type m_group_by{ group_type::none };
        common::filter_type m_filter{};
        std::size_t m_max_file_size_mb{ 0 };

    public:
        options() = default;

        options(on_duplicate duplicate, group_type group_by, common::filter_type filter, std::size_t max_file_size_mb);

        options& set_on_duplicate(on_duplicate duplicate);

        options& set_group_type(group_type group_by);

        options& set_filter_type(common::filter_type filter);

        std::size_t get_max_file_size_mb() const;

        options& set_max_file_size_mb(std::size_t mb_value);

        friend class downloader;
    };
} // boorudl

#endif //BOORUDL_OPTIONS_H
