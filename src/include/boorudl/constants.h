#ifndef LIBBOORUDL_CONSTANTS_H
#define LIBBOORUDL_CONSTANTS_H

#include <string_view>

namespace boorudl::constants {
    constexpr inline std::string_view posts_path{ "index.php?page=dapi&s=post&q=index" };

    constexpr inline std::string_view comments_path{ "index.php?page=dapi&s=comment&q=index&post_id=" };

    constexpr inline std::string_view favorites_path{ "index.php?page=favorites&s=view&id=" };
} // boorudl::constants

#endif //LIBBOORUDL_CONSTANTS_H
