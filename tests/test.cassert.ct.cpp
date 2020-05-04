#include <chk/cassert.hpp>

namespace {
constexpr int fun(int i) {
	chk::cassert(i > 0, "Hello");
	chk::cassert(i > 0);
	return i;
}
}  // namespace

#if  defined(__clang_major__) && __clang_major__ != 9) // ACTUALLY ONLY RELEASE HAS ISSUES
#error "error"
#else
static_assert(fun(-1) == -1, "no cassert triggered"); // clang-6, release, does not fail
#endif
int main() {
}
