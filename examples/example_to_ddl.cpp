#include <vimg/to_ddl.hpp>
#include <iostream>

using namespace vi::to_ddl;

struct MyType {
    int foo;
    double bar;
    std::string_view baz;
    bool qux;
    std::nullptr_t null;
};

int main() {
    constexpr auto ddl = generate_type_ddl<MyType>();
    std::cout << std::string_view(ddl) << std::endl;
    return 0;
}
