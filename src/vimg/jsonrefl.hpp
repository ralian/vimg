#include <concepts>
#include <meta>
#include <ctre.hpp>
#include <string_view>
#include <array>
#include <algorithm>
#include <ranges>
#include <span>
#include <initializer_list>

namespace vi::jsonrefl {

namespace re {
using namespace ctre::literals;
// This is greedy - add dyke language?
constexpr auto object = R"!(\s*\{([^\}]*)\}\s*)!"_ctre;

constexpr auto string = R"!(\s*"([^"]*)"\s*)!"_ctre;

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

// Extract all keys from a JSON object at compile time
template<std::size_t MaxKeys = 64>
consteval auto extract_keys(const std::string_view json_obj) {
    std::array<std::string_view, MaxKeys> keys{};
    std::size_t count = 0;
    
    std::string_view remaining = json_obj;

    while (auto string_match = re::key_value.search(remaining))
    {
        keys[count++] = string_match.get<1>().to_view();
        remaining = remaining.substr(string_match.get<0>().to_view().size());
    }
    
    return std::make_pair(keys, count);
}

// Make member spec from key
consteval auto make_member_spec_from_key(std::string_view key) {
    std::meta::data_member_options opts{};
    opts.name = key;
    return std::meta::data_member_spec(^^std::string_view, opts);
}

// Build member specs from array of keys
template<std::size_t N>
consteval auto build_member_specs(const std::array<std::string_view, N>& keys, std::size_t count) {
    std::array<std::meta::info, N> specs{};
    for (std::size_t i = 0; i < count; ++i) {
        // Use helper to create each spec
        specs[i] = make_member_spec_from_key(keys[i]);
    }
    return specs;
}

// Define an aggregate type from a JSON object
// Usage: 
//   struct MyType;
//   consteval { define_aggregate_from_json(^^MyType, json_string); }
consteval auto define_aggregate_from_json(
    std::meta::info type_class,
    const std::string_view json_str)
{
    auto obj_content_opt = extract_object(json_str);
    if (!obj_content_opt.has_value()) {
        return type_class;
    }
    
    auto obj_content = *obj_content_opt;
    constexpr auto max_keys = std::size_t{64};
    auto keys_result = extract_keys<max_keys>(obj_content);
    auto& keys = keys_result.first;
    auto key_count = keys_result.second;
    
    auto member_specs = build_member_specs(keys, key_count);
    
    // Create span with valid members only
    std::span<const std::meta::info> spec_span(member_specs.data(), key_count);
    
    return std::meta::define_aggregate(type_class, spec_span);
}

}
