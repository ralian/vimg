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

constexpr std::string_view json_2 = R"(
{
    "foo": [1, 2, 3, 4],
    "bar": {
        "nested_key": "nested_val"
    }
}
)";

constexpr std::string_view json_3 = R"(
{
    "foo": "baz1",
    "bar": "baz2"
}
)";

// Define the type (cannot generate identifiers with reflection)
struct JsonType1;
consteval {
    define_aggregate_from_json(^^JsonType1, json_1);
}
// The type is now complete with a definition.

int main() {
    // Inspect JsonType1 using reflection
    constexpr auto type_info = ^^JsonType1;
    constexpr auto type_name = std::meta::identifier_of(type_info);
    
    printf("=== JsonType1 Type Information ===\n");
    printf("Type name: %.*s\n", 
           (int)type_name.size(),
           type_name.data());
    
    printf("Sizeof(JsonType1): %zu bytes\n", sizeof(JsonType1));
    
    JsonType1 obj1;
    printf("obj1.foo = %.*s\n", (int)obj1.foo.size(), obj1.foo.data());
    printf("obj1.bar = %.*s\n", (int)obj1.bar.size(), obj1.bar.data());
    printf("obj1.baz = %.*s\n", (int)obj1.baz.size(), obj1.baz.data());
    
    return 0;
}
