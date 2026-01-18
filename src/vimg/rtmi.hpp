#include <array>
#include <meta>
#include <string_view>
#include <variant>

namespace vimg::rtmi {

constexpr std::size_t MAX_BASES = 16;

struct runtime_meta_info {
    const std::string_view identifier;
    const std::array<std::string_view, MAX_BASES> bases;
    const std::size_t bases_count;

    // todo operator== / operator<=>

    template<typename T> 
    constexpr runtime_meta_info(const T* _ = nullptr) :
        identifier(std::meta::identifier_of(^^T)),
        bases(init_bases<T>()),
        bases_count(get_base_count<T>())
    {
    }

    constexpr auto bases_view() const {
        return std::span<const std::string_view>(bases.data(), bases_count);
    }

private:
    //consteval auto ctx = std::meta::access_context::unprivileged();

    template<typename T>
    static constexpr std::size_t get_base_count() {
        constexpr auto ctx = std::meta::access_context::current();
        if constexpr (std::ranges::sized_range<decltype(std::meta::bases_of(^^T, ctx))>) {
            return static_cast<std::size_t>(std::ranges::size(std::meta::bases_of(^^T, ctx)));
        } else {
            return std::size_t{0};
        }
    }

    template<typename T>
    static constexpr auto init_bases() {
        constexpr std::size_t size = get_base_count<T>();
        std::array<std::string_view, MAX_BASES> result{};
        std::size_t index = 0;
        constexpr auto ctx = std::meta::access_context::current();
        for (const auto& base_info : std::meta::bases_of(^^T, ctx)) {
            if (index < MAX_BASES) {
                result[index++] = std::meta::identifier_of(base_info);
            }
        }
        return result;
    }
};

template <typename ...Ts> constexpr
std::variant<Ts*...> dynamic_meta_cast(const runtime_meta_info& rtmi, void* ptr)
{
    const auto rtmi_options = {runtime_meta_info(static_cast<Ts*>(nullptr))...};
    std::variant<Ts*...> vars[] = {static_cast<Ts*>(ptr)...};
    size_t idx = 0;
    for (const auto option : rtmi_options) {
        if (option.identifier == rtmi.identifier)
            return vars[idx];
        idx++;
    }
    return std::variant<Ts*...>();
}

// can this be replaced with std::any?
class rtmi_base_type {
public:
    void foo() {printf("rtmi_base_type::foo()\n");}
};

struct weak_meta_ptr {
    rtmi_base_type* ptr;
    runtime_meta_info rtmi;

    // todo: Use a concept to enforce derived from rtmi_base_type
    template<typename T> constexpr weak_meta_ptr(T* ptr) :
        ptr(static_cast<rtmi_base_type*>(ptr)),
        rtmi(runtime_meta_info(ptr))
    {}
};

}
