# loopify red/green tests

Tests for `jank::ir::opt::loopify` — see the spec + non-cascade proof in
[`TeglonLabs/jank-crane`](https://github.com/TeglonLabs/jank-crane) (`loopify-spec.md`).

## simonw red/green discipline (why this dir exists before the pass works)

> "Writing code is cheap now" — don't *argue* that loopify is correct and non-cascading,
> *encode it as a test and run it.* (Agentic Engineering Patterns: Red/green TDD, First run the tests.)

1. **First run the tests** — baseline jank's existing suite (`test/bash/clojure-test-suite`)
   BEFORE and AFTER any loopify work. Non-cascade ⟺ the suite is green both times with
   `--loopify` OFF (default) AND ON.
2. **Red** — `deep-non-tail-recursion.jank` overflows the native stack today; with
   `--loopify` it currently *still* overflows (the pass declines on unproven shapes). RED.
3. **Green** — implement the tail then non-tail rewrite in `src/cpp/jank/ir/opt/loopify.cpp`
   until `jank --loopify run deep-non-tail-recursion.jank` prints `500000500000`, AND the
   full suite stays green with the flag both off and on.

## Differential obligation (spec L5)

For every loopified `f`: `∀x. f(x) == loopify(f)(x)` (equal result AND equal effect order).
The flag-gated, default-off design means a regression can only ever surface under `--loopify`,
never in the default build — the blast radius is the flag.
