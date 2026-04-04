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

// Inheritance edge styles: protected base → dashed, private base → dotted.
struct InheritDemoBase {
    int base_field;
};

struct ProtectedInherit : protected InheritDemoBase {
    int mid_field;
};

struct PrivateInherit : private InheritDemoBase {
    int leaf_field;
};

struct MemberAccessDemo {
public:
    int pub_field;
protected:
    int prot_field;
private:
    int priv_field;
};

int main() {
    constexpr auto dot_oversized = generate_dot_for_types<
        Base, Derived, MoreDerived, DisjointType, DoubleDerived,
        InheritDemoBase, ProtectedInherit, PrivateInherit, MemberAccessDemo>();
    constexpr auto dot = vi::rightsize<decltype(dot_oversized), dot_oversized>();
    std::cout << std::string_view(dot) << std::endl;
    return 0;
}
