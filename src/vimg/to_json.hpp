#include <meta>
#include <array>
#include <string_view>
#include <vimg/util.hpp>

namespace vi::to_json {

template<typename T>
consteval auto type_to_json_type()
{
    if (std::is_same_v<T, bool>) {
        return "boolean";
    } else if (std::is_integral_v<T> || std::is_floating_point_v<T>) {
        return "number";
    } else if (std::is_convertible_v<T, std::string_view>) {
        return "string";
    } else if (std::is_same_v<T, std::nullptr_t>) {
        return "null";
    } else if (std::is_array_v<T>) {
        return "array";
    } else {
        return "object";
    }
}

template<std::size_t N, typename U>
constexpr void append_json_value(char_arr<N>& result, const U& value) {
    if constexpr (std::is_same_v<U, bool>) {
        result += value ? "true" : "false";
    } else if constexpr (std::is_integral_v<U>) {
        char buf[32];
        auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), value);
        if (ec == std::errc{}) { result += std::string_view(buf, ptr - buf); }
    } else if constexpr (std::is_floating_point_v<U>) {
        char buf[32];
        auto [ptr, ec] = to_chars_floating_point(buf, value);
        if (ec == std::errc{}) { result += std::string_view(buf, ptr - buf); }
    } else if constexpr (std::is_convertible_v<U, std::string_view>) {
        result += '"';
        std::string_view s(value);
        for (char c : s) {
            if (c == '\\' || c == '"') { result += '\\'; result += c; }
            else { result += c; }
        }
        result += '"';
    } else {
        result += "null";
    }
}

template<typename T, std::size_t N = 256>
consteval auto generate_type_json()
-> char_arr<N> {
    char_arr<N> result;
    
    bool first = true;
    constexpr auto ctx = std::meta::access_context::current();
    template for (auto& member : 
        std::define_static_array(std::meta::nonstatic_data_members_of(^^T, ctx))
    ) {
        result += (first ? "{" : ", ");
        first = false;
        
        result += '"';
        result += std::meta::identifier_of(member);
        result += "\": \"";
        result += std::meta::display_string_of(std::meta::type_of(member));
        result += '"';
    }

    result += '}';
    return result;
}

template<typename T, std::size_t N = 256>
consteval auto generate_object_json(const T& obj)
-> char_arr<N> {
    char_arr<N> result;
    result += (std::is_array_v<T> ? "[" : "{");

    bool first = true;
    constexpr auto ctx = std::meta::access_context::current();
    template for (auto& member : 
        std::define_static_array(std::meta::nonstatic_data_members_of(^^T, ctx))
    ) {
        if (!first) result += ", ";
        first = false;

        result += '"';
        result += std::meta::identifier_of(member);
        result += "\": ";
        append_json_value(result, obj.[:member:]);
        
    }

    result += (std::is_array_v<T> ? "]" : "}");
    return result;
}

}
