#pragma once

#include <ctre.hpp>
#include <meta>

namespace vi::validator {

namespace pattern {
    using namespace ctre::literals;
    constexpr ctll::fixed_string camelCase = R"!([a-z][a-zA-Z0-9]*)!";
    constexpr ctll::fixed_string snake_case = R"!([a-z][a-z0-9_]*)!";
}

template<typename T, ctll::fixed_string Pattern>
consteval void validate_class_members()
{
    constexpr auto ctx = std::meta::access_context::current();
    template for (auto& type :
        std::define_static_array(std::meta::members_of(^^T, ctx))
    ) {
        static_assert(!std::meta::has_identifier(type)
                    || !std::meta::is_user_declared(type)
                    || ctre::match<Pattern>.match(std::meta::identifier_of(type)),
            "Type does not match pattern");
    }
}

};
