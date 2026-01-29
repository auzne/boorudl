#ifndef LIBBOORUDL_PAGE_H
#define LIBBOORUDL_PAGE_H

#include "boorudl/post.h"

#include <pugixml.hpp>
#include <vector>

namespace boorudl {
    class page : public std::vector<post> {
    private:
        bool m_is_missing_ids{ false };

    public:
        page() = default;

        explicit page(pugi::xml_node page_xml);

        bool is_missing_ids() const;

        page& set_is_missing_ids(bool value);
    };
} // boorudl

#endif //LIBBOORUDL_PAGE_H
