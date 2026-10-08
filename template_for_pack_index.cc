// Reduced from fast_io's print layer (print_freestanding.h):
// "iterating expansion statements are not yet supported" on the
// Oct 8 llvm_herbceptions build; the same source compiles on the
// Oct 5 llvm_native_with_gcc build.
//
// broken:  toolchains_build/llvm_herbceptions/build/bin/clang++ -std=c++26
// working: softwares/llvm_native_with_gcc/bin/clang++ -std=c++26
#include <array>
#include <cstddef>

template <::std::size_t first, ::std::size_t last>
consteval auto make_range() noexcept
{
	::std::array<::std::size_t, last - first> arr{};
	::std::size_t i{first};
	for (auto &e : arr)
	{
		e = i++;
	}
	return arr;
}

template <::std::size_t first, ::std::size_t last>
inline constexpr auto index_range{make_range<first, last>()};

template <typename... Args>
consteval ::std::size_t first_eight_byte() noexcept
{
	template for (constexpr auto i : index_range<0, sizeof...(Args)>)
	{
		using arg = Args...[i];
		if constexpr (sizeof(arg) == 8)
		{
			return i;
		}
	}
	return sizeof...(Args);
}

static_assert(first_eight_byte<char, long, int>() == 1);
static_assert(first_eight_byte<char, short>() == 2);

int main() {}
