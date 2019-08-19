#pragma once

#include <exception>
#include <cstdlib>

#include <chk/detail/utils.hpp>

// chk::terminate
// terminates unconditionally compilation when invoked
// calls std::terminate at runtime
namespace chk {
#if defined(__clang_major__)
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Winvalid-noreturn"
#elif defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4297)
#endif
[[noreturn]] inline constexpr void cterminate(bool dummy = true) noexcept {
	if (dummy) {
		std::terminate();
	}
}
[[noreturn]] inline constexpr void cabort(bool dummy = true) noexcept {
	if (dummy) {
		std::abort();
	}
}
#if defined(__clang_major__)
#	pragma clang diagnostic pop
#elif defined(_MSC_VER)
#	pragma warning(pop)
#endif

}  // namespace chk
