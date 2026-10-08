// Control case: plain & on the same dependent function-template
// specialization deduces into the pack fine — the bug is specific to
// __builtin_addressof producing a value the pack deduction sees as
// "(no value)".
struct state {};

template <typename A, typename T>
void invoke(void *, A, T) {}

template <typename T, typename S, typename... Args>
T *make_state(S sched, Args &&...args)
{
	return nullptr;
}

template <typename Alloc>
void submit(Alloc sched)
{
	make_state<state>(sched, &invoke<Alloc, int>, 0);
}

int main()
{
	submit(0);
}
