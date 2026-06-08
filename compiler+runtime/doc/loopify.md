# loopify red/green tests

Tests for `jank::ir::opt::loopify` — see the spec + non-cascade proof in
[`TeglonLabs/jank-crane`](https://github.com/TeglonLabs/jank-crane) (`loopify-spec.md`).

## simonw red/green discipline (why this dir exists before the pass works)

> "Writing code is cheap now" — don't *argue* that loopify is correct and non-cascading,
> *encode it as a test and run it.* (Agentic Engineering Patterns: Red/green TDD, First run the tests.)

1. **First run the tests** — baseline jank's existing suite (`test/bash/clojure-test-suite`)
   BEFORE and AFTER any loopify work. Non-cascade ⟺ the suite is green both times with
   `--loopify` OFF (default) AND ON.
2. **Red** — `skip-deep-non-tail-recursion.jank` overflows the native stack today; with
   `--loopify` it currently *still* overflows (the pass declines on unproven shapes). RED.
3. **Green** — implement the tail then non-tail rewrite in `src/cpp/jank/ir/opt/loopify.cpp`
   until `jank --loopify run deep-non-tail-recursion.jank` prints `500000500000`, AND the
   full suite stays green with the flag both off and on.

## Witnessed on real jank (2026-06-08)

Building this fork via `nix build .#jank-release` compiled the scaffold and ran the suite. Two facts,
observed (not asserted):
- the C++ scaffold (`ir/opt/loopify.{hpp,cpp}`, the `processor.cpp` gate, the `cli.hpp` flag) **compiles
  into real jank** — the build reached the test phase.
- running this file segfaulted the test binary: `deep-non-tail-recursion.jank => Segmentation fault: 11`
  — i.e. deep non-tail recursion **stack-overflows actual jank**. That IS the red, on the real impl.

Because a crashing file aborts the whole `jank-test` binary, this file carries the `skip-` prefix (jank's
harness skips it). When `ir/opt/loopify.cpp` implements the transform, rename to
`pass-loopify-deep-recursion.jank` and run it under `--loopify` (must print `500000500000`, no crash).

## Differential obligation (spec L5)

For every loopified `f`: `∀x. f(x) == loopify(f)(x)` (equal result AND equal effect order).
The flag-gated, default-off design means a regression can only ever surface under `--loopify`,
never in the default build — the blast radius is the flag.
