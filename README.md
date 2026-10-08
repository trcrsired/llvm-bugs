# Reduced clang bugs (fork: trcrsired/llvm-project, -fherbceptions capable builds)

Compilers referenced:
- `~/toolchains_build/llvm_herbceptions/build/bin/clang++` — Oct 8 build ("broken")
- `~/softwares/llvm_native_with_gcc/bin/clang++` — Oct 5 build ("working")

## 1. template_for_pack_index.cc — `template for` unsupported on Oct 8 build

    clang++ -std=c++26 -fsyntax-only template_for_pack_index.cc

- Oct 8 herbceptions build: `error: iterating expansion statements are
  not yet supported` — the feature regressed/dropped between the Oct 5
  and Oct 8 builds.
- Oct 5 build: compiles clean.
- Context: breaks fast_io's print layer (`print_freestanding.h`,
  `concat.h`, `to_cxx26.h` use `template for` + pack indexing
  `Args...[i]`).

## 2. template_for_pack_crash.cc — ICE on ill-formed expansion statement

    clang++ -std=c++26 -fsyntax-only template_for_pack_crash.cc

- **Both** builds segfault: `args...` is rejected as an expansion range,
  then `if constexpr` inside the body is evaluated on the error type —
  crash in `ASTContext::getTypeInfoImpl` via
  `CheckCXXBooleanCondition`/`getIntegerConstantExpr`.

## 3. addressof_deduction_min.cc — __builtin_addressof of a function
   template specialization defeats pack deduction

    clang++ -std=c++26 -fsyntax-only addressof_deduction_min.cc

- **Both** builds: `deduced incomplete pack <(no value), int> for
  template parameter 'Args'` — `__builtin_addressof(invoke<int,int>)`
  produces an expression whose deduction yields no value.
- Control files (compile clean):
  - `addressof_deduction_ok.cc` — plain `&invoke<Alloc,int>` works.
  - `addressof_deduction_nondep.cc` — `__builtin_addressof` on a
    non-template function works.
