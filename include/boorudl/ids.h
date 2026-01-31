#ifndef BOORUDL_IDS_H
#define BOORUDL_IDS_H

#include "boorudl/common.h"
#include "boorudl/tags.h"

#include <initializer_list>
#include <set>

namespace boorudl {
    class ids : public std::set<common::id_type> {
    private:
        using id_batch_type = std::vector<std::vector<common::id_type>>;

    public:
        ids() = default;

        ids(std::initializer_list<common::id_type> ids);

        tags to_tags() const;

    private:
        std::string id_to_string(common::id_type id) const;
    };
} // boorudl

#endif //BOORUDL_IDS_H
