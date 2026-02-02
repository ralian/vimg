# VirtualImage

Vimg is, presently, a header-only collection of c++26 reflection utilites. Reflection is quite new, but support exists in new versions of both GCC and Clang.

Nothing special is required to build vimg, other than a supporting compiler. Here is the requisite cmake to link vimg (as mentioned it's header only, but this will set your target's include dirs correctly):

```
# I suggest using FetchContent before this point to install the project.
find_project(vimg REQUIRED)

add_executable(myexe)
target_link_libraries(myexe PRIVATE vimg)
```

If you need to install a new version of GCC, I have provided a dockerfile for this - see the next section.

## Using the GCC Dockerfile

```
docker build . -f gcc-trunk.docker --build-arg JOBS=4 -t gcc-builder:latest
docker run --rm -v .:/vimg -it gcc-builder
```

## Building the examples

```
cmake --preset default
cmake --build --preset debug
```

## How do I use the project?

First of all, there is no way to import the entire project at once. Each header/module/sub-namespace of `vi` should be seen as its own tool, and you should pull in whichever you want to do the job you need. I haven't done any benchmarking as yet, but I do anticipate that some of these reflection tricks will add to compile times quite significantly... So, for instance, to use Runtime Meta-info, simply `#include <vi/rtmi.hpp>` and have at it.

## Project Synopsis

- [Runtime Meta-Info](./src/vimg/rtmi.hpp): RTMI was inspired by RTTI, a longstanding feature of C++. Even though Reflection is a compile-time feature, this library allows you to store the reflection info and retrieve it at runtime. It could be used for dynamic feature and binary structure discovery.
- [To DDL](./src/vimg/to_ddl.hpp): Generate a DDL `CREATE TABLE` expression from a C++ type's members.
- [To DOT (Graphviz)](./src/vimg/to_dot.hpp): Generate a Graphviz Digraph of a given pack of types, showing each type as a node with all members and edges directed to bases of the type.
- [To JSON](./src/vimg/to_json.hpp): JSON Generation utilites:
    - Generate a JSON schema from a C++ type's members
    - Generate a JSON object for said type (todo: We still can't populate these with values)
- [From JSON](./src/vimg/from_json.hpp): This was inspired in part by [json2cpp](https://github.com/lefticus/json2cpp) by Jason Turner - his project builds a variant tree from a given input JSON, but this tool allows you to create an aggregate representing a given object. Uses CTRE to aid in describing a compile-time JSON parser. This unsurprisingly has some limitations:
    - Cannot generate initializers or constructors
    - Cannot generate the identifier of your type from JSON (See the [example](./examples/example_from_json.cpp) for how to use this.)
    - Only a subset of JSON is supported: Namely the language of a single non-nested JSON Object. This is a regular language and makes things easier to parse. I suspect you could build a nested parser and use a pack to define multiple types at once, but I see no value in doing this... yet.

