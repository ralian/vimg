#include <vimg/to_ddl.hpp>
#include <iostream>

using namespace vi::to_ddl;

struct MyType {
    int foo;
    double bar;
    std::string_view baz;
    bool qux;
    std::nullptr_t nullval;
};

int main() {
    constexpr auto ddl_oversized = generate_type_ddl<MyType>();
    constexpr auto ddl = vi::rightsize<decltype(ddl_oversized), ddl_oversized>();
    std::cout << std::string_view(ddl) << std::endl;

    constexpr MyType obj{.foo = 1, .bar = 2.0, .baz = "baz", .qux = true, .nullval = nullptr};
    constexpr auto insert_oversized = generate_insert_statement(obj);
    constexpr auto insert = vi::rightsize<decltype(insert_oversized), insert_oversized>();
    std::cout << std::string_view(insert) << std::endl;
    
    return 0;
}
