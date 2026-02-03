#pragma once

#include <array>
#include <cmath>
#include <meta>
#include <string_view>
#include <system_error>

namespace vi {

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

constexpr std::size_t strlen(const char* str) noexcept
{
    std::size_t result = 0;
    while (str[result] != '\0') {
        result++;
    }
    return result;
}

template<typename U> requires std::is_floating_point_v<U>
constexpr auto to_chars_floating_point(char* buf, U value)
{
    constexpr int sig_digits = 15;
    std::size_t n = 0;
    U v = value;
    if (std::isnan(static_cast<double>(v))) {
        buf[n++] = 'n'; buf[n++] = 'a'; buf[n++] = 'n';
    } else if (std::isinf(static_cast<double>(v))) {
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

    std::errc ec = std::errc{};
    return std::make_pair(buf + n, ec);
}

template<std::size_t N>
struct char_arr {
    std::array<char, N> data;

    constexpr char_arr() : data{} {}

    constexpr char_arr(const char* str) : data{str, str + strlen(str)} {}

    constexpr char_arr<N>&operator+=(char c)
    {
        std::size_t size = strlen(data.data());
        data[size] = c;
        data[size+1] = '\0';
        return *this;
    }

    constexpr char_arr<N>& operator+=(const std::string_view& str)
    {
        std::size_t size = strlen(data.data());
        std::copy(str.begin(), str.end(), data.begin() + size);
        data[size + str.size()] = '\0';
        return *this;
    }

    constexpr operator std::string_view() const {
        return std::string_view(data.data(), strlen(data.data()));
    }
};

} // namespace vi
