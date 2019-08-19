#pragma once

#include <chk/cterminate.hpp>
#include <chk/detail/source_location.hpp>
#include <chk/detail/utils.hpp>

#include <cstdlib>
#include <cstdio>
#include <exception>

// chk::unreachable
// in constexpr context, terminates unconditionally when path is reached
// in runtime context, triggers ub when path reached at runtime
//
// while chk::cterminate/chk::cabort unconditionally terminates, chk::unreachable should be used for marking dead code paths
// it is thus more equivalent to chk::cassert(false, "path reached") / assert(false && "path reached");
// It was inspired by __builtin_unreachable, which is not constepxr and not avaiable on msvc
// check P1429 for reasoning about abort or terminate
namespace chk {
#if defined(__clang_major__)
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Winvalid-noreturn"
#endif
[[noreturn]] inline constexpr void unreachable(
     const char* msg = "", chk::detail::source_location sl = chk::detail::source_location(), bool dummy = true) noexcept {
	if (dummy) {
		chk::detail::stop_compilation_in_constexpr_context();
#if defined(NDEBUG)
		(void) sl;
		(void) msg;
#else
		std::fflush(stdout);
		std::fprintf(stderr, "Reached %s:%i of function \"%s\"\n", sl.file_name(), static_cast<int>(sl.line()), sl.function_name());
		if (msg[0] != '\0') {
			std::fprintf(stderr, "%s\n", msg);
		}
#endif
	}
#if !defined(NDEBUG)
	chk::cabort();
#endif
}
#if defined(__clang_major__)
#	pragma clang diagnostic pop
#endif

}  // namespace chk
