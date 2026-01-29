#include "boorudl/page.h"
#include "boorudl/utility.h"

#include <pugixml.hpp>
#include <string_view>

namespace boorudl {
    page::page(pugi::xml_node page_xml) {
        std::string_view first_node_name{ page_xml.first_child().name() };
        if (first_node_name == "posts")
            page_xml = page_xml.child("posts");

        reserve(utility::xml_node_size(page_xml));

        for (pugi::xml_node post : page_xml.children()) {
            emplace_back(post);
        }
    }

    bool page::is_missing_ids() const {
        return m_is_missing_ids;
    }

    page& page::set_is_missing_ids(bool value) {
        m_is_missing_ids = value;

        return *this;
    }
} // boorudl