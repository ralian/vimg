#include <meta>
#include <array>
#include <string_view>
#include <vimg/char_arr.hpp>

namespace vi::to_json {

template<typename T, std::size_t N = 256>
consteval auto generate_type_json_for_aggregate()
-> char_arr<N> {
    char_arr<N> result;
    
    auto ctx = std::meta::access_context::current();
    bool first = true;
    for (const auto& member : std::meta::nonstatic_data_members_of(^^T, ctx)) {
        result += (first ? '{' : ',');
        first = false;
        
        result += '"';
        auto name = std::meta::identifier_of(member);
        result += name;
        result += "\": \"";
        auto type = std::meta::display_string_of(std::meta::type_of(member));
        result += type;
        result += '"';
    }

    result += '}';
    return result;
}

}
