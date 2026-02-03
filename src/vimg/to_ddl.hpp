#include <meta>
#include <array>
#include <charconv>
#include <cmath>
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

template<std::size_t N, typename U>
constexpr void append_sql_value(char_arr<N>& result, const U& value) {
    if constexpr (std::is_same_v<U, bool>) {
        result += value ? "1" : "0";
    } else if constexpr (std::is_integral_v<U>) {
        char buf[32];
        auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), value);
        if (ec == std::errc{}) { result += std::string_view(buf, ptr - buf); }
    } else if constexpr (std::is_floating_point_v<U>) {
        constexpr int sig_digits = 15;
        char buf[64];
        std::size_t n = 0;
        U v = value;
        if (std::isnan(v)) {
            buf[n++] = 'n'; buf[n++] = 'a'; buf[n++] = 'n';
        } else if (std::isinf(v)) {
            if (v < 0) buf[n++] = '-';
            buf[n++] = 'i'; buf[n++] = 'n'; buf[n++] = 'f';
        } else if (v == 0) {
            buf[n++] = '0';
        } else {
            if (v < 0) { buf[n++] = '-'; v = -v; }
            int exp10 = static_cast<int>(std::floor(std::log10(v)));
            U scale = 1;
            if (exp10 > 0) {
                for (int i = 0; i < exp10; ++i) scale /= 10;
            } else if (exp10 < 0) {
                for (int i = 0; i < -exp10; ++i) scale *= 10;
            }
            U mantissa = v * scale;
            int first = static_cast<int>(mantissa);
            buf[n++] = static_cast<char>('0' + first);
            buf[n++] = '.';
            mantissa -= first;
            for (int i = 0; i < sig_digits - 1; ++i) {
                mantissa *= 10;
                int d = static_cast<int>(mantissa);
                buf[n++] = static_cast<char>('0' + d);
                mantissa -= d;
            }
            buf[n++] = 'e';
            if (exp10 < 0) { buf[n++] = '-'; exp10 = -exp10; }
            char exp_buf[8];
            std::size_t exp_n = 0;
            do {
                exp_buf[exp_n++] = static_cast<char>('0' + (exp10 % 10));
                exp10 /= 10;
            } while (exp10);
            while (exp_n) buf[n++] = exp_buf[--exp_n];
        }
        result += std::string_view(buf, n);

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
