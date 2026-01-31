#include "boorudl/common.h"
#include "boorudl/ids.h"

#include <initializer_list>
#include <set>
#include <string>

namespace boorudl {
    const std::string id_prefix{ "id:" };

    ids::ids(std::initializer_list<common::id_type> ids)
        : std::set<common::id_type>{ ids } { }

    tags ids::to_tags() const {
        std::set<std::string> ids_str;

        for (const auto& id : *this) {
            ids_str.insert(id_to_string(id));
        }
        
        tags tags;
        tags.add_or(ids_str);

        return tags;
    }

    std::string ids::id_to_string(common::id_type id) const {
        return id_prefix + std::to_string(id);
    }
} // boorudl