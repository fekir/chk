#pragma once

#include <type_traits>

namespace chk {
namespace detail {

#if defined(__GNUC__)
#	pragma GCC diagnostic push
#	pragma GCC diagnostic ignored "-Wunused-const-variable"
#endif
#if !defined(NDEBUG)
constexpr bool cassert_active = true;
#else
constexpr bool cassert_active = false;
#endif
#if defined(__GNUC__)
#	pragma GCC diagnostic pop
#endif

// necessary for supporting c++14
struct lambda_expr {
	bool e;
	inline constexpr bool operator()() const noexcept {
		return e;
	}
};
struct lambda_msg {
	const char* c;
	inline constexpr const char* operator()() const noexcept {
		return c;
	}
};

// support both const char* and std::string
inline constexpr const char* zstr(const char* s) noexcept {
	return s;
}

template<class T> inline constexpr auto zstr(const T& s) noexcept -> decltype(s.c_str()) {
	return s.c_str();
}

template<class T> inline constexpr auto call_or_convert_to_str_like_i(T&& obj, int) noexcept -> decltype(zstr(obj)) {
	return zstr(obj);
}
template<class T> inline constexpr auto call_or_convert_to_str_like_i(T&& obj, long) -> decltype(obj()) {
	return obj();
}

// decltype(auto) is more correct, but does not work on msvc
template<class T> inline constexpr decltype(auto) call_or_convert_to_str_like(T&& obj) {
	return call_or_convert_to_str_like_i(obj, 0);
}

// stops compilation because not constexpr
inline void stop_compilation_in_constexpr_context() noexcept {
}

// c++20 adds std::is_constant_evaluated, while this does not work on all compilers
#if __cplusplus <= 201703L
template<class T> constexpr void test_constexpr_helper(T&&) {
}
#endif

// converts obj to bool in following order
// implicit/explicit conversion to bool/converts implictely to some other integral type
// operator!
// call operator
template<class T> inline constexpr auto call_or_convert_to_bool_i(T&& obj, int) -> decltype(!!obj, bool()) {
	return !!obj;
}
template<class T> inline constexpr auto call_or_convert_to_bool_i(T&& obj, long) -> decltype(obj(), bool()) {
	return obj();
}
template<class T> inline constexpr bool call_or_convert_to_bool(T&& obj) {
	return call_or_convert_to_bool_i(obj, 0);
}
}  // namespace detail
}  // namespace chk
