#include <cstdio>
#include <vimg/rtmi.hpp>

using namespace vimg::rtmi;

constinit std::tuple<std::string_view, int> int_type {"int", 0};

class object : public rtmi_base_type {
public:
    void foo() {printf("object::foo()\n");}
};

class example_a : public object {
public:
    void foo() {printf("example_a::foo()\n");}
};

class example_b : public object {
public:
    void foo() {printf("example_b::foo()\n");}
};

struct event_handler {
    int (*handle_event) (int);
    // int event_trampoline(int i) {
    //     handle_event(i);
    // }
};

class event_emitter {
    std::vector<event_handler*> handlers;

public:
    void register_handler(event_handler* eh) { handlers.push_back(eh); }
    void fire_event() {
        int i = 1;
        for (auto* handler : handlers)
            int result = handler->handle_event(i);
    }
};

struct handle_event_a : event_handler {
    int (*impl) (int) = +[](int in){
        printf("handle_event_a::handle_event(%d)\n", in);
        return 0;
    };

    handle_event_a() {handle_event = impl;}
} hea;

struct handle_event_b : event_handler {
    int (*impl) (int) = +[](int in){
        printf("handle_event_b::handle_event(%d)\n", in);
        return 0;
    };

    handle_event_b() {handle_event = impl;}
} heb;

int main() {
    object o;
    example_a a;
    example_b b;
    const auto meta_ptrs = {weak_meta_ptr{&o}, weak_meta_ptr{&a}, weak_meta_ptr{&o}, weak_meta_ptr{&b}};
    for (const auto& meta_ptr : meta_ptrs) {
        //printf("%s : %s\n", meta_ptr.rtmi.identifier.data(), meta_ptr.rtmi.bases[0].data());
        auto v = dynamic_meta_cast<object, example_a, example_b>(meta_ptr.rtmi, meta_ptr.ptr);
        std::visit([](auto* t){t->foo();}, v);
    }

    event_emitter ee;
    ee.register_handler(&hea);
    ee.register_handler(&heb);
    ee.fire_event();

    return 0;
}
