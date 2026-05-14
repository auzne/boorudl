#ifndef BOORUDL_ENUMS_H
#define BOORUDL_ENUMS_H

#include <string>
#include <string_view>

namespace boorudl {
    namespace sort {
        enum class type {
            id,
            score,
            rating,
            user,
            height,
            width,
            parent,
            source,
            random,
        };

        enum class order {
            Default,
            asc,
            desc,
        };

        std::string type_to_string(type type);

        std::string order_to_string(order order);

        std::string build_tag(type type, order order);
    }

    namespace rating {
        enum class type {
            safe,
            questionable,
            Explicit,
        };

        type parse(std::string_view value);

        std::string to_string(type type);

        std::string build_tag(type type);
    }
}

#endif //BOORUDL_ENUMS_H
