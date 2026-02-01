#include <meta>
#include <array>
#include <string_view>
#include <utility>
#include <vimg/char_arr.hpp>

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

template<typename T>
consteval std::size_t count_members() {
    return std::meta::nonstatic_data_members_of(
        ^^T, std::meta::access_context::current()).size();
}

template<typename T, std::size_t I>
consteval auto get_member() {
    return std::meta::nonstatic_data_members_of(
        ^^T, std::meta::access_context::current())[I];
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

}
