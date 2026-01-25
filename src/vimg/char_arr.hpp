#include <array>
#include <string_view>

namespace vi {

constexpr std::size_t strlen(const char* str) noexcept
{
    std::size_t result = 0;
    while (str[result] != '\0') {
        result++;
    }
    return result;
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

}