// Minimal: __builtin_addressof of a function template specialization —
// dependent OR not — fails template argument deduction into a pack with
// "(no value)". Plain & and non-template functions are unaffected.
struct state {};

template <typename A, typename T>
void invoke(A, T) {}

template <typename T, typename S, typename... Args>
T *make_state(S sched, Args &&...args)
{
	return nullptr;
}

int main()
{
	// FAILS on both builds: deduced incomplete pack <(no value), int>
	make_state<state>(0, __builtin_addressof(invoke<int, int>), 0);
}
