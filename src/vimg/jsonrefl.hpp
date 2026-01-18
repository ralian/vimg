#include <concepts>
#include <meta>
#include <ctre.hpp>
#include <string_view>

namespace vi::jsonrefl {

// I know `numeric` in JSON is more complex, but for now we only support int
template<typename T>
concept is_json_type = std::same_as<T, std::string_view>
    || std::same_as<T, bool>
    || std::same_as<T, std::nullptr_t>
    || std::same_as<T, int>;

namespace re {
using namespace ctre::literals;
// Using \X* instead of .* would be more correct here but won't compile with CTRE
constexpr auto object = R"!(\s*\{(.*)\}\s*)!"_ctre;
constexpr auto string = R"!(\s*"(\w*)"\s*:\s*"(.*)"\s*,?)!"_ctre;
//constexpr auto numeric = R"!(\s*"(\w*)"\s*:\s*([-+]\d*)\s*)!"_ctre;
}

constexpr std::optional<std::string_view>
    extract_object(const std::string_view in)
{
    if (auto m = re::object.match(in))
        return m.get<1>().to_view();
    else
        return std::nullopt;
}

constexpr std::optional<std::tuple<std::string_view, std::string_view>>
    extract_string(const std::string_view in)
{
    if (auto m = re::string.match(in))
        return std::make_tuple(m.get<1>().to_view(), m.get<2>().to_view());
    else
        return std::nullopt;
}

}
