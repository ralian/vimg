#include <vimg/validator.hpp>

using namespace vi::validator;

struct test {
    static void fooBar() {printf("fooBar\n");}
    static void fooBar2() {printf("fooBar2\n");}
};

struct test2 {
    static void foo_bar() {printf("foo_bar\n");}
    static void foo_bar_2() {printf("foo_bar_2\n");}
};

int main() {
    validate_class_members<test, pattern::camelCase>();
    validate_class_members<test2, pattern::snake_case>();
    return 0;
}
