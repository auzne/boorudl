#include "boorudl/enums.h"

#include <map>
#include <string>

namespace boorudl {
    namespace sort {
        std::string type_to_string(type type) {
            switch (type) {
                case type::id:
                    return "id";
                case type::score:
                    return "score";
                case type::rating:
                    return "rating";
                case type::user:
                    return "user";
                case type::height:
                    return "height";
                case type::width:
                    return "width";
                case type::parent:
                    return "parent";
                case type::source:
                    return "source";
                case type::random:
                    return "random";
            }
            return {};
        }

        std::string order_to_string(order order) {
            switch (order) {
                case order::asc:
                    return "asc";
                case order::desc:
                    return "desc";
                case order::Default:
                default:
                    return {};
            }
        }

        std::string build_tag(type type, order order) {
            auto base{ "sort:" + type_to_string(type) };
            return order != order::Default
                ? base + ':' + order_to_string(order)
                : base;
        }
    }

    namespace rating {
        type parse(std::string_view value) {
            static const std::map<std::string_view, type> rates{
                { "s",            type::safe },
                { "safe",         type::safe },
                { "q",            type::questionable },
                { "questionable", type::questionable },
                { "e",            type::Explicit },
                { "explicit",     type::Explicit },
            };

            return rates.at(value);
        }

        std::string to_string(type type) {
            switch (type) {
                case type::safe:
                    return "safe";
                case type::questionable:
                    return "questionable";
                case type::Explicit:
                    return "explicit";
            }
            return {};
        }

        std::string build_tag(type type) {
            return "rating:" + to_string(type);
        }
    }
}