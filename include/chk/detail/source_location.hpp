#pragma once

#include <cstdint>

namespace chk {
namespace detail {

// __builtin() is a gcc extension (provided by clang 9 too), msvc does not have anythign similar
// look at https://en.cppreference.com/w/cpp/experimental/source_location

#if defined(_MSC_VER)
#	define MISSING_CONSTEXPR_BUILTIN_LOCATION
#elif defined(__clang_major__)
    #if  __clang_major__ < 9
    #	define MISSING_CONSTEXPR_BUILTIN_LOCATION
    #else
    // notice that clang9 defines __GNUC__ < 7
    #endif
#elif (defined(__GNUC__) && __GNUC__ < 7)
#	define MISSING_CONSTEXPR_BUILTIN_LOCATION
#else
// FIXME: gcc <=6, it's not constexpr!
#	if __GNUC__ > 6
static_assert(__builtin_LINE() == __LINE__, "__builtin_LINE does not behave as expected.");
#	endif
#endif

#ifdef MISSING_CONSTEXPR_BUILTIN_LOCATION
constexpr char const* __builtin_FUNCTION() noexcept {
	return "function";
}
constexpr char const* __builtin_FILE() noexcept {
	return "file";
}
constexpr int __builtin_LINE() noexcept {
	return 0;
}
#endif

struct source_location {
	constexpr explicit source_location(
	     const char* function_ = __builtin_FUNCTION(), const char* file_ = __builtin_FILE(), int line_ = __builtin_LINE()) noexcept :
	            function{function_},
	            file{file_}, m_line{line_} {
	}
	constexpr const char* file_name() const noexcept {
		return file;
	}
	constexpr const char* function_name() const noexcept {
		return function;
	}
	constexpr std::uint_least32_t line() const noexcept {
		return static_cast<std::uint_least32_t>(m_line);
	}

private:
	const char* function;
	const char* file;
	int m_line;
};
}  // namespace detail
}  // namespace chk
