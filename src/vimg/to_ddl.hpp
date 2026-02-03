#include <meta>
#include <array>
#include <charconv>
#include <string_view>
#include <utility>
#include <vimg/util.hpp>

namespace vi::to_ddl {

template<typename T>
consteval auto type_to_ddl_type()
{
    if (std::is_same_v<T, bool>) {
        return "BOOLEAN";
    } else if (std::is_floating_point_v<T>) {
        return "REAL";
    } else if (std::is_integral_v<T>) {
        return "INTEGER";
    } else if (std::is_convertible_v<T, std::string_view>) {
        return "TEXT";
    } else {
        return "UNKNOWN";
    }
}

template<std::size_t N, typename U>
constexpr void append_sql_value(char_arr<N>& result, const U& value) {
    if constexpr (std::is_same_v<U, bool>) {
        result += value ? "TRUE" : "FALSE";
    } else if constexpr (std::is_integral_v<U>) {
        char buf[32];
        auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), value);
        if (ec == std::errc{}) { result += std::string_view(buf, ptr - buf); }
    } else if constexpr (std::is_floating_point_v<U>) {
        char buf[32];
        auto [ptr, ec] = to_chars_floating_point(buf, value);
        if (ec == std::errc{}) { result += std::string_view(buf, ptr - buf); }
    } else if constexpr (std::is_convertible_v<U, std::string_view>) {
        result += '\'';
        std::string_view s(value);
        for (char c : s) {
            if (c == '\'') { result += '\''; result += '\''; }
            else { result += c; }
        }
        result += '\'';
    } else {
        result += "NULL";
    }
}

template<typename T, std::size_t N = 256>
consteval auto generate_type_ddl()
-> char_arr<N> {
    char_arr<N> result;
    result += "CREATE TABLE ";
    result += std::meta::identifier_of(^^T);
    result += " (\n";

    constexpr std::size_t NumMembers = count_members<T>();
    [&]<std::size_t... Is>(std::index_sequence<Is...>) {
        ( (result += "\t",
           result += std::meta::identifier_of(get_member<T, Is>()),
           result += "\t\t",
           result += type_to_ddl_type<typename [:std::meta::type_of(get_member<T, Is>()):]>(),
           result += ",\n"), ... );
    }(std::make_index_sequence<NumMembers>{});

    result += ");";
    return result;
}

template<typename T, std::size_t N = 256>
consteval auto generate_insert_statement(const T& obj)
-> char_arr<N> {
    char_arr<N> result;
    result += "INSERT INTO ";
    result += std::meta::identifier_of(^^T);
    result += " VALUES (";

    constexpr std::size_t NumMembers = count_members<T>();
    [&]<std::size_t... Is>(std::index_sequence<Is...>) {
        ((append_sql_value(result, obj.[:get_member<T, Is>():]),
          (Is + 1 < NumMembers ? (void)(result += ", ") : (void)0)), ...);
    }(std::make_index_sequence<NumMembers>{});

    result += ");";
    return result;
}

}
