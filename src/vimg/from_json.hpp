#pragma once

#include <concepts>
#include <meta>
#include <ctre.hpp>
#include <string_view>
#include <array>
#include <algorithm>
#include <ranges>
#include <span>
#include <initializer_list>
#include <tuple>

namespace vi::from_json {

namespace re {
using namespace ctre::literals;
// This is greedy - add dyke language?
constexpr auto object = R"!(\s*\{([^\}]*)\}\s*)!"_ctre;

constexpr auto string = R"!(\s*"([^"]*)"\s*)!"_ctre;
constexpr auto numeric = R"!(\s*(\d+)\s*)!"_ctre;
constexpr auto boolean = R"!(\s*(true|false)\s*)!"_ctre;
constexpr auto null = R"!(\s*null\s*)!"_ctre;

// We are using \w+ here instead of \X+ or [^"] to restrict our character set.
constexpr auto key_value = R"!(\s*"(\w+)"\s*:\s*([^,}]*)\s*,?)!"_ctre;
}

constexpr std::optional<std::string_view>
    extract_object(const std::string_view in)
{
    if (auto m = re::object.match(in))
        return m.get<1>().to_view();
    else
        return std::nullopt;
}

constexpr std::optional<std::string_view>
    extract_string(const std::string_view in)
{
    if (auto m = re::string.match(in))
        return m.get<1>().to_view();
    else
        return std::nullopt;
}

template<std::size_t MaxKeys = 64>
consteval auto extract_keys_and_values(const std::string_view json_obj) {
    std::array<std::string_view, MaxKeys> keys{};
    std::array<std::string_view, MaxKeys> values{};
    std::size_t count = 0;
    
    std::string_view remaining = json_obj;

    while (auto string_match = re::key_value.search(remaining))
    {
        keys[count] = string_match.get<1>().to_view();
        values[count] = string_match.get<2>().to_view();
        count++;
        remaining = remaining.substr(string_match.get<0>().to_view().size());
    }
    
    return std::make_tuple(keys, values, count);
}

consteval auto determine_value_type(std::string_view value) {
    if (re::numeric.match(value)) {
        return ^^int;
    } else if (re::boolean.match(value)) {
        return ^^bool;
    } else if (re::null.match(value)) {
        return ^^std::nullptr_t;
    } else if (re::string.match(value)) {
        return ^^std::string_view;
    }
    // If ill-formed
    return ^^void;
}

template<typename T>
constexpr void parse_value(T& out, std::string_view value) {
    if constexpr (std::is_same_v<T, bool>) {
        out = (value == "true");
    } else if constexpr (std::is_integral_v<T>) {
       std::from_chars(value.data(), value.data() + value.size(), out);
    } else if constexpr (std::is_same_v<T, std::nullptr_t>) {
        out = nullptr;
    } else if constexpr (std::is_same_v<T, std::string_view>) {
        out = value;
    } else {
        static_assert(false, "Invalid type");
    }
}

consteval auto make_member_spec_from_key_value(std::string_view key, std::string_view value) {
    std::meta::data_member_options opts{};
    opts.name = key;
    auto value_type = determine_value_type(value);
    return std::meta::data_member_spec(value_type, opts);
}

template<std::size_t N>
consteval auto build_member_specs(
    const std::array<std::string_view, N>& keys,
    const std::array<std::string_view, N>& values,
    std::size_t count) {
    std::array<std::meta::info, N> specs{};
    for (std::size_t i = 0; i < count; ++i) {
        specs[i] = make_member_spec_from_key_value(keys[i], values[i]);
    }
    return specs;
}

// Define an aggregate type from a JSON object
// Usage: 
//   struct MyType;
//   consteval { define_aggregate_from_json(^^MyType, json_string); }
consteval auto define_aggregate_from_json(
    std::meta::info type_class,
    const std::string_view json_str
) {
    auto obj_content_opt = extract_object(json_str);
    if (!obj_content_opt.has_value()) {
        return type_class;
    }
    
    auto obj_content = *obj_content_opt;
    constexpr auto max_keys = std::size_t{64};
    auto [keys, values, key_count] = extract_keys_and_values<max_keys>(obj_content);
    
    auto member_specs = build_member_specs(keys, values, key_count);
    
    // Create span with valid members only
    std::span<const std::meta::info> spec_span(member_specs.data(), key_count);
    
    return std::meta::define_aggregate(type_class, spec_span);
}

// Parse JSON (single object for now) into a given type
template <typename T>
constexpr T parse_json(
    const std::string_view json_str
) {
    T out;

    auto obj_content_opt = extract_object(json_str);
    if (!obj_content_opt.has_value()) {
        throw std::runtime_error("Invalid JSON object");
    }
    
    auto obj_content = *obj_content_opt;
    constexpr auto max_keys = std::size_t{64};
    auto [keys, values, key_count] = extract_keys_and_values<max_keys>(obj_content);
    
    constexpr auto ctx = std::meta::access_context::current();
    template for (const auto& member : 
        std::define_static_array(std::meta::nonstatic_data_members_of(^^T, ctx))
    ) {
        for (std::size_t i = 0; i < key_count; ++i) {
            if (std::meta::identifier_of(member) == keys[i]) {
                parse_value(out.[:member:], values[i]);
                break;
            }
        }
    }

    return std::move(out);
}

template <typename T, T Val>
struct defaults
{
    constexpr T& operator()()
    {
        return Val;
    }
};

} // namespace vi::from_json
