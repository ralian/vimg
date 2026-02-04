#pragma once

// #include <memory>
#include <meta>

namespace vi::from_pyclass {

// namespace re {
//     using namespace ctre::literals;

//     constexpr auto class_def = R"!(\s*class\s*([^\s{}]+)\s*([^\}]*)\s*)!"_ctre;

//     constexpr auto string = R"!(\s*"([^"]*)"\s*)!"_ctre;
//     constexpr auto number = R"!(\s*(-?\d+(\.\d+)?)\s*)!"_ctre;
//     constexpr auto boolean = R"!(\s*(True|False)\s*)!"_ctre;
//     constexpr auto none = R"!(\s*None\s*)!"_ctre;

//     // We are using \w+ here instead of \X+ or [^"] to restrict our character set.
//     constexpr auto member_type_value = R"!(\t(\w+)\s*:\s*(\w+)\s+=\s*(\w+))!"_ctre;
// }

template<typename T, typename Agg>
struct derived : public T {
    Agg data;
};

struct base {};

using my_derived = derived<base, my_agg>;

template<typename Base, typename Agg>
consteval auto make_derived_specialization_info()
{
    return ^^derived<Base, Agg>;
}

// template<std::size_t N>
// consteval auto from_pyclass(std::string_view classdef, std::array<std::meta::info, N> possible_bases) {
//     auto member_specs = extract_member_specs(classdef);
//     auto requested_base = extract_requested_base(classdef);
//     if (requested_base.has_value()) {
//         auto base_index = std::find_if(possible_bases.begin(), possible_bases.end(), [&](const auto& base) {
//             return std::meta::identifier_of(base) == requested_base.value();
//         });
//         if (base_index != possible_bases.end()) {
//             return 
//             using TX = derived<typename [: *base_index :]>;
//             std::meta::define_aggregate(^^TX::internal_aggregate, member_specs);
//             return ^^TX;
//         }
//     }

    
//     return std::meta::define_aggregate(^^T, member_specs);
// }

} // namespace vi::from_pyclass
