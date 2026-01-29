#ifndef LIBBOORUDL_IDS_H
#define LIBBOORUDL_IDS_H

#include "boorudl/common.h"
#include "boorudl/exporter.h"
#include "boorudl/page.h"
#include "boorudl/source.h"

#include <initializer_list>
#include <set>

namespace boorudl {
    class ids : public std::set<common::id_type> {
    public:
        ids() = default;

        ids(std::initializer_list<common::id_type> ids);

        page to_page(const source& source) const;

        page to_page(const source& source, exporter_ref exporter) const;
    };
} // boorudl

#endif //LIBBOORUDL_IDS_H
