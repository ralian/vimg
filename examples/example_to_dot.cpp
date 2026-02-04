#include <vimg/to_dot.hpp>

#include <iostream>

using namespace vi::to_dot;

struct Base {
    int id;
};

struct Derived : public Base {
    std::string name;
    bool active;
};

struct MoreDerived : public Derived {
    double value;
};

struct DisjointType {
    int foo;
};

struct DoubleDerived : public Derived, public DisjointType {
    int moar_members;
};

int main() {
    constexpr auto dot = generate_dot_for_types<Base, Derived, MoreDerived, DisjointType, DoubleDerived>();
    std::cout << std::string_view(dot) << std::endl;
    return 0;
}
