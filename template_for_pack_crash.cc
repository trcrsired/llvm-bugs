// ICE (segfault) on both builds — parser/sema error recovery on an
// ill-formed expansion statement. `args...` is not a valid expansion
// range, but the diagnostic path then evaluates `if constexpr` on an
// error type and dereferences it in ASTContext::getTypeInfoImpl.
//
//   clang++ -std=c++26 -fsyntax-only template_for_pack_crash.cc
//
// Stack: ParseForStatement -> ActOnCondition -> CheckCXXBooleanCondition
//        -> Expr::getIntegerConstantExpr -> ASTContext::getTypeInfoImpl
#include <cstddef>

template <typename... Args>
consteval ::std::size_t count_eight_byte(Args... args) noexcept
{
	::std::size_t n{};
	template for (constexpr auto const &a : args...) // invalid range
	{
		if constexpr (sizeof(a) == 8) // evaluated on error type -> ICE
		{
			++n;
		}
	}
	return n;
}

static_assert(count_eight_byte(1, 1L) == 1);

int main() {}
