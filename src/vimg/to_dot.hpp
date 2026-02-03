#pragma once

#include <meta>
#include <array>
#include <string_view>
#include <vimg/util.hpp>

namespace vi::to_dot {

template<std::size_t N>
constexpr void escape_dot(char_arr<N>& out, std::string_view str) {
    out += '"';
    for (char c : str) {
        if (c == '\\' || c == '"' || c == '<' || c == '>' || c == ':')
            out += '\\';
        out += c;
    }
    out += '"';
}

template<typename T, std::size_t LabelSize = 512>
consteval auto build_node_label() -> char_arr<LabelSize> {
    char_arr<LabelSize> result;
    result += std::meta::display_string_of(^^T);
    constexpr auto ctx = std::meta::access_context::current();
    for (const auto& member : std::meta::nonstatic_data_members_of(^^T, ctx)) {
        result += "|";
        result += std::meta::identifier_of(member);
        result += " : ";
        result += std::meta::display_string_of(std::meta::type_of(member));
    }
    return result;
}

template<typename T, std::size_t NodeSize = 1024>
consteval auto build_node() -> char_arr<NodeSize> {
    char_arr<NodeSize> result;
    result += "\t";
    escape_dot(result, std::meta::identifier_of(^^T));
    result += " [label=";
    escape_dot(result, std::string_view(build_node_label<T>()));
    result += ", shape=record];\n";
    return result;
}

template<typename T, std::size_t EdgeSize = 256>
consteval auto build_edges() -> char_arr<EdgeSize> {
    char_arr<EdgeSize> result;
    constexpr auto ctx = std::meta::access_context::current();
    for (const auto& base : std::meta::bases_of(^^T, ctx)) {
        result += "  ";
        escape_dot(result, std::meta::identifier_of(^^T));
        result += " -> ";
        escape_dot(result, std::meta::identifier_of(base));
        result += ";\n";
    }
    return result;
}

template<typename T, std::size_t BlockSize = 1024>
consteval auto dot_for_type()
-> char_arr<BlockSize>
{
    char_arr<BlockSize> result;
    result += std::string_view(build_node<T>());
    result += std::string_view(build_edges<T>());
    return result;
}

template<typename... Ts>
consteval auto generate_dot_for_types() -> char_arr<8192> {
    char_arr<8192> result;
    result += "digraph G {\nrankdir=LR;\n";
    (result += std::string_view(dot_for_type<Ts>()), ...);
    result += "}\n";
    return result;
}

}
