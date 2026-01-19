#include <iostream>
#include <meta>
#include <ranges>
#include <vimg/from_json.hpp>

using namespace vi::jsonrefl;

constexpr std::string_view json_1 = R"(
{
    "foo": "lorem",
    "bar": "ipsum",
    "baz": "sit"
}
)";

// Define the type (cannot generate identifiers with reflection)
struct JsonType1;
consteval {
    define_aggregate_from_json(^^JsonType1, json_1);
}
// The type is now complete with a definition.


constexpr std::string_view json_2 = R"(
{
    "string_type": "string_value",
    "int_type": 123,
    "bool_type": true,
    "null_type": null
}
)";

struct JsonType2;
consteval {
    define_aggregate_from_json(^^JsonType2, json_2);
}

constexpr std::string_view json_3 = R"(
{
    "foo": [1, 2, 3, 4],
    "bar": {
        "nested_key": "nested_val"
    }
}
)";

template<typename T>
void inspect_json_type() {
    constexpr auto type_name = std::meta::identifier_of(^^T);
    std::cout << "=== " << type_name << " Type Information ===\n";
    std::cout << "Sizeof(" << type_name << "): " << sizeof(^^T) << " bytes\n";
    std::cout<< std::endl;
}

int main() {
    inspect_json_type<JsonType1>();
    JsonType1 obj1;
    std::cout << "obj1.foo = " << obj1.foo << '\n';
    std::cout << "obj1.bar = " << obj1.bar << '\n';
    std::cout << "obj1.baz = " << obj1.baz << '\n';
    std::cout << std::endl;

    inspect_json_type<JsonType2>();
    JsonType2 obj2{.string_type = "string_value", .int_type = 123, .bool_type = true, .null_type = nullptr};
    std::cout << "obj2.string_type = " << obj2.string_type << '\n';
    std::cout << "obj2.int_type = " << obj2.int_type << '\n';  
    std::cout << "obj2.bool_type = " << obj2.bool_type << '\n';
    std::cout << "obj2.null_type = " << obj2.null_type << '\n';
    std::cout << std::endl;
    return 0;
}
