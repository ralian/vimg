#include <vimg/to_json.hpp>

#include <iostream>

using namespace vi::to_json;

struct MyType {
    int foo;
    std::string_view bar;
    bool baz;
};

using MyType2 = std::array<int, 5>;

int main() {
    constexpr auto json_view_oversized = generate_type_json<MyType>();
    constexpr auto json_view = vi::rightsize<decltype(json_view_oversized), json_view_oversized>();
    std::cout << std::string_view(json_view) << std::endl;

    // constexpr auto json_view2 = generate_type_json<MyType2>();
    // std::cout << std::string_view(json_view2) << std::endl;

    constexpr MyType obj{.foo = 1, .bar = "bar", .baz = true};
    constexpr auto object_json_oversized = generate_object_json(obj);
    constexpr auto object_json = vi::rightsize<decltype(object_json_oversized), object_json_oversized>();
    std::cout << std::string_view(object_json) << std::endl;

    constexpr MyType2 obj2{1, 4, 9, 16, 25};
    constexpr auto object_json2_oversized = generate_object_json(obj2);
    constexpr auto object_json2 = vi::rightsize<decltype(object_json2_oversized), object_json2_oversized>();
    std::cout << std::string_view(object_json2) << std::endl;

    return 0;
}

