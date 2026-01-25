#include <vimg/to_json.hpp>

#include <iostream>

using namespace vi::to_json;

struct MyType {
    int foo;
    std::string bar;
    bool baz;
};

int main() {
    constexpr auto json_view = generate_type_json_for_aggregate<MyType>();
    std::cout << std::string_view(json_view) << std::endl;
    return 0;
}

