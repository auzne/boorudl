#ifndef LIBBOORUDL_ENUMS_H
#define LIBBOORUDL_ENUMS_H

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
        };

        std::string to_string(type type);

        std::string build_tag(type type, bool ascending);
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

#endif //LIBBOORUDL_ENUMS_H
