#include "boorudl/common.h"
#include "boorudl/ids.h"

#include <initializer_list>
#include <set>
#include <string>
#include <vector>

namespace boorudl {
    const std::string id_prefix{ "id:" };

    ids::ids(std::initializer_list<common::id_type> ids)
        : std::set<common::id_type>{ ids } { }

    tags ids::to_tags() const {
        if (this->empty())
            return {};

        std::set<std::string> ids_str;

        for (const auto& id : *this) {
            ids_str.insert(id_to_string(id));
        }
        
        tags tags;
        tags.add_or(ids_str);

        return tags;
    }

    ids::id_batch_type ids::to_batches(common::id_type items_per_batch) const {
        // if already has less or equal items than batch just return as id_batch_type
        const auto size{ this->size() };
        if (size <= items_per_batch)
            return id_batch_type{ *this };

        // reserve enough batches
        id_batch_type result;
        result.reserve((size / items_per_batch) + 1);

        // creates batches in a for-each...
        ids current;
        for (const auto& id : *this) {
            // if has enough items push to result and create new batch
            if (current.size() >= items_per_batch) {
                result.push_back(current);
                current = {};
            }

            current.insert(id);
        }

        return result;
    }

    std::string ids::id_to_string(common::id_type id) const {
        return id_prefix + std::to_string(id);
    }
} // boorudl