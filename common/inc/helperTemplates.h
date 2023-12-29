#include <concepts>  // IWYU pragma: keep
#include <iostream>
#include <memory>
#include <string>

#include <cxxabi.h>

template <class T>
struct is_streamable {
    static std::false_type test(...);

    template <class U>
    static auto test(const U& u) -> decltype(std::declval<std::ostream&>() << u, std::true_type{});

    static constexpr bool value = decltype(test(std::declval<T>()))::value;
};

template <class T>
inline constexpr bool is_streamable_v = is_streamable<T>::value;

template <template <typename, typename...> class ContainerType, typename ValueType, typename... Args>
std::enable_if_t<!is_streamable_v<ContainerType<ValueType, Args...>>, std::ostream&> operator<<(std::ostream& os, ContainerType<ValueType, Args...>& container) {
    os << "{";
    bool commaFlag = false;
    for (auto const& obj : container) {
        os << (commaFlag ? ", " : "") << obj;
        commaFlag = true;
    }
    os << "}";
    return os;
}

template <typename T>
concept IterableContainer = requires(T a) {
    { a.begin() } -> std::input_iterator;
    { a.end() } -> std::input_iterator;
    { a.cbegin() } -> std::input_iterator;
    { a.cend() } -> std::input_iterator;
} && !std::same_as<T, std::string> && !std::same_as<T, std::string_view>;

template <IterableContainer T>
std::ostream& operator<<(std::ostream& os, const T& container) {
    os << "{";
    bool commoFlag = false;
    for (auto const& obj : container) {
        os << (commoFlag ? ", " : "") << obj;
        commoFlag = true;
    }
    os << "}";
    return os;
}

template <typename T>
auto demangle() -> std::string {
    int  status         = -1;
    auto demangled_name = std::unique_ptr<char, void (*)(void*)>{
        abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status), std::free};
    return (status == 0) ? demangled_name.get() : "unknown";
}
