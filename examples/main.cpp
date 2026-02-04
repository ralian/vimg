#include <iostream>
#include <meta>

struct MyType {
    int foo;
    double bar;
};

int main() {
    constexpr auto r = ^^int;
    [:r:] x = 42;
    [:^^char:] y = 'a';
    std::cout << x << std::endl;
    std::cout << y << std::endl;

    MyType obj1;
    std::cout << std::meta::identifier_of(^^obj1) << std::endl;
    
    MyType obj2;
    std::cout << std::meta::display_string_of(^^obj2) << std::endl;

    MyType obj3{.foo = 1, .bar = 2.0};
    std::cout << std::meta::display_string_of(^^obj3.[:foo:]) << std::endl;

    return 0;
}
