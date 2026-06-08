# jank ⟕ crane convergence — loopify

This fork of [jank-lang/jank](https://github.com/jank-lang/jank) carries the **loopify** pass,
a port of [bloomberg/crane](https://github.com/bloomberg/crane)'s `src/loopify.ml`
(Rocq/Gallina → C++ extraction) into jank-IR.

Full study, IR maps, and non-cascade proof live in the hub:
**[TeglonLabs/jank-crane](https://github.com/TeglonLabs/jank-crane)** — see `loopify-spec.md`,
`simonw-workflow.md`, and `roots/{q,plus,minus,crane-vs-dafny}.md`.

## What loopify does
Converts self-recursive jank functions into iteration so deep recursion does not overflow the
native stack — the one capability jank lacks that crane already solved. jank's *tail* case is
already handled by `loop`/`named_recursion`/`binding_shadows`; loopify adds (a) auto-detection of
plain self-recursion and (b) the non-tail frame-stack transform.

## Files
- `compiler+runtime/include/cpp/jank/ir/opt/loopify.hpp` — contract
- `compiler+runtime/src/cpp/jank/ir/opt/loopify.cpp`     — conservative skeleton (identity until implemented)
- `compiler+runtime/test/jank/loopify/`                  — red/green tests
- wired into `ir/processor.cpp` behind `util::cli::opts.loopify` (**default OFF**)

## Non-cascade in one line
The pass is a free function `void loopify(function&)` gated by a default-off CLI flag, operating
in place per-function, emitting only pre-existing IR opcodes. With the flag off (the default) it is
never called; with it on it declines on any shape it cannot prove safe. Blast radius = the flag.
