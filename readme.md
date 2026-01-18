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

docker build . -f gcc-trunk.docker --build-arg JOBS=4 -t gcc-builder:latest
docker run --rm -v .:/vimg -it gcc-builder

## How do I use the project?

First of all, there is no way to import the entire project at once. Each header/module/sub-namespace of `vi` should be seen as its own tool, and you should pull in whichever you want to do the job you need. I haven't done any benchmarking as yet, but I do anticipate that some of these reflection tricks will add to compile times quite significantly... So, for instance, to use Runtime Meta-info, simply `import/include <vi/rtmi.hpp>` and have at it.

Of course, even though this library is header only, I'd like to remind you that you can embed it in its own translation unit to achieve the same effect as a linked library - that will prevent this from affecting your _overall_ compile times.

## Project Synopsis

- [Runtime Meta-Info](): RTMI was inspired by RTTI, a longstanding feature of C++. Even though Reflection is a compile-time feature, this library allows you to store the reflection info and retrieve it at runtime. It could be used for dynamic feature and binary structure discovery.
- [Json Reflector](): Does what it says on the tin. This was inspired in part by a project by Jason Turner - his project builds a variant tree from a given input JSON, but this tool allows you to create a tuple representing a given object. Uses CTRE to aid in describing a compile-time JSON parser.

