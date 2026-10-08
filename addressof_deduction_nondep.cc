// Narrowing further: __builtin_addressof of a *non-dependent* function
// template specialization also fails pack deduction -> the bug is in
// __builtin_addressof's expression type on functions, not dependence.
struct state {};

void invoke(int) {}

template <typename T, typename S, typename... Args>
T *make_state(S sched, Args &&...args)
{
	return nullptr;
}

int main()
{
	make_state<state>(0, __builtin_addressof(invoke), 0);  // fails: (no value)
	make_state<state>(0, &invoke, 0);                      // works
}
