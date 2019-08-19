#pragma once

#include <chk/cterminate.hpp>
#include <chk/detail/source_location.hpp>
#include <chk/detail/utils.hpp>

#if !defined(NDEBUG)
#	include <cstdlib>
#	include <cstdio>
#	include <exception>
#endif

#if __cplusplus > 201703L
#	include <type_traits>
// clang is a pita, does not have std::is_constant_evaluated.... disable test for now
#endif

// chk::cassert
// similar to assert macro, calls abort when expression is false
// it is not a macro, thus arguments are evaluated on the caller side also when NDEBUG is defined
// to avoid expensive calculation, use lambdas, in this case, similar to the assert macro, arguments are not evaluated at runtime if NDEBUG is defined
// if NDEBUG is defined, all checks are disabled at runtime
// if CNDEBUG then all checks are disabled at compiletime too
// NOTICE: on clang, wth c++ < 20, compile-time-checks are disabled too
namespace chk {

template<class F, class M>
inline constexpr void cassert(F expr, M msg_, chk::detail::source_location sl = chk::detail::source_location()) noexcept {
#if defined(CNDEBUG)
	(void) expr;
	(void) sl;
	(void) msg_;
#else

// FIXME: only if expr is bool/lamba is constexpr!
#	if __cplusplus > 201703L
	constexpr bool is_constexpr = std::is_constant_evaluated();
#	elif (defined(__GNUC__) && __GNUC__ >= 9) || (defined(__clang_major__) && __clang_major__ >= 9)
	constexpr bool is_constexpr = __builtin_is_constant_evaluated();
#	else
	// might not work correctly, even if it should with c++14 and c++17
	constexpr bool is_constexpr = noexcept(chk::detail::test_constexpr_helper(expr));
#	endif

	if ((chk::detail::cassert_active || is_constexpr) && !chk::detail::call_or_convert_to_bool(expr)) {
#	if defined(NDEBUG)
		(void) sl;
		(void) msg_;
#	else
		chk::detail::stop_compilation_in_constexpr_context();
		std::fflush(stdout);
		std::fprintf(stderr, "Assertion failed on file %s, line %d, function %s\n", sl.file_name(), static_cast<int>(sl.line()),
		     sl.function_name());
		const auto& msg = chk::detail::call_or_convert_to_str_like(msg_);
		if (msg[0] != '\0') {
			std::fprintf(stderr, "%s\n", chk::detail::zstr(msg));
		}
#	endif
		chk::cabort();
	}
#endif
}
template<class F>
inline constexpr void cassert(F expr, const char* msg = "", chk::detail::source_location sl = chk::detail::source_location()) noexcept {
	return cassert(expr, chk::detail::lambda_msg{msg}, sl);
}
}  // namespace chk
