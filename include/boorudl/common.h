#ifndef BOORUDL_COMMON_H
#define BOORUDL_COMMON_H

#include <cstddef>
#include <functional>

namespace boorudl {
    class post;

    namespace common {
        using id_type = std::size_t;
        using filter_type = std::function<bool(const post&)>;
    }
}

#endif //BOORUDL_COMMON_H
