#include <vimg/jsonrefl.hpp>

constexpr std::string_view json_1 = R"(
{
    "foo": "lorem",
    "bar": "ipsum",
    "baz": "sit",
    "shall_i_pass": false,
    "MMXVI": 2016,
    "nothing": null
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
    "foo bar": "baz1",
    "foo.bar": "baz2"
}
)";

int main() {

}
