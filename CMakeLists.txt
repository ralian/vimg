cmake_minimum_required(VERSION 3.23)
project(vimg LANGUAGES CXX VERSION 0.1)

# Can't just set CMAKE_CXX_STANDARD just yet
add_compile_options(-std=c++26 -freflection)

include(FetchContent)
FetchContent_Declare(ctre SYSTEM EXCLUDE_FROM_ALL
    GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
    GIT_TAG v3.10.0
)
FetchContent_MakeAvailable(ctre)

add_library(vimg INTERFACE)
target_include_directories(vimg INTERFACE src)
target_link_libraries(vimg INTERFACE ctre)

add_executable(example_rtmi examples/example_rtmi.cpp)
target_link_libraries(example_rtmi PRIVATE vimg)

add_executable(example_to_json examples/example_to_json.cpp)
target_link_libraries(example_to_json PRIVATE vimg)

add_executable(example_to_ddl examples/example_to_ddl.cpp)
target_link_libraries(example_to_ddl PRIVATE vimg)

add_executable(example_from_json examples/example_from_json.cpp)
target_link_libraries(example_from_json PRIVATE vimg)

add_executable(example_to_dot examples/example_to_dot.cpp)
target_link_libraries(example_to_dot PRIVATE vimg)

add_executable(example_validator examples/example_validator.cpp)
target_link_libraries(example_validator PRIVATE vimg)

add_executable(example_from_pyclass examples/example_from_pyclass.cpp)
target_link_libraries(example_from_pyclass PRIVATE vimg)
