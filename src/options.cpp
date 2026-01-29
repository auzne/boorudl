#include "boorudl/options.h"

#include <utility>

namespace boorudl {
    options::options(on_duplicate duplicate, group_type group_by, common::filter_type filter)
        : m_duplicate{ duplicate }
        , m_group_by{ group_by }
        , m_filter{ std::move(filter) } { }

    options& options::set_on_duplicate(on_duplicate duplicate) {
        m_duplicate = duplicate;
        return *this;
    }

    options& options::set_group_type(group_type group_by) {
        m_group_by = group_by;
        return *this;
    }

    options& options::set_filter_type(common::filter_type filter) {
        m_filter = filter;
        return *this;
    }
} // boorudl