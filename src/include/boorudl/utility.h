#ifndef LIBBOORUDL_UTILITY_H
#define LIBBOORUDL_UTILITY_H

#include <iterator>
#include <pugixml.hpp>

namespace boorudl::utility {
    inline long xml_node_size(const pugi::xml_node& node) {
        return std::distance(node.begin(), node.end());
    }
} // boorudl::utility

#endif //LIBBOORUDL_UTILITY_H
