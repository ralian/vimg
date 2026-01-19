#include <vimg/jsonrefl.hpp>
#include <meta>
#include <cstdio>
#include <ranges>

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
constexpr void inspect_json_type() {
    constexpr auto type_name = std::meta::identifier_of(^^T);
    printf("=== %.*s Type Information ===\n", 
           (int)type_name.size(),
           type_name.data());
    printf("Sizeof(%.*s): %zu bytes\n",
           (int)type_name.size(),
           type_name.data(), sizeof(^^T));
}

int main() {
    inspect_json_type<JsonType1>();
    JsonType1 obj1;
    printf("obj1.foo = %.*s\n", (int)obj1.foo.size(), obj1.foo.data());
    printf("obj1.bar = %.*s\n", (int)obj1.bar.size(), obj1.bar.data());
    printf("obj1.baz = %.*s\n", (int)obj1.baz.size(), obj1.baz.data());

    inspect_json_type<JsonType2>();
    JsonType2 obj2{.string_type = "string_value", .int_type = 123, .bool_type = true, .null_type = nullptr};
    printf("obj2.string_type = %.*s\n", (int)obj2.string_type.size(), obj2.string_type.data());
    printf("obj2.int_type = %d\n", obj2.int_type);
    printf("obj2.bool_type = %d\n", obj2.bool_type);
    printf("obj2.null_type = %d\n", obj2.null_type);
    
    return 0;
}
