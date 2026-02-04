#include <vimg/from_pyclass.hpp>
#include <iostream>

using namespace vi::from_pyclass;

struct my_agg;
consteval {
    std::meta::define_aggregate(^^my_agg, {});
}
using my_derived = [: make_derived_specialization_info<base, my_agg>() :];

int main() {
    my_derived d = {.i = 1};
    std::cout << d.i << std::endl;
    return 0;
}
