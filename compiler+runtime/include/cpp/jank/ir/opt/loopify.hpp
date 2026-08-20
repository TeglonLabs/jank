#pragma once

/* loopify — convert self-recursive jank functions into iteration, to avoid
 * native-stack overflow on deep inputs. Port of bloomberg/crane's loopify.ml
 * (Rocq/Gallina -> C++ extraction) to jank-IR.
 * Spec + non-cascade proof: TeglonLabs/jank-crane : loopify-spec.md.
 *
 * Non-cascade contract (default-OFF; opt-in via util::cli::opts.loopify):
 *   - Operates ONLY on `function &fn`, in place, per-function (matches ir/opt peers
 *     remove_nops / hoist_literals / hoist_var_derefs).
 *   - CONSUMES only {call, named_recursion, branch, jump, ret}; PRODUCES only the
 *     PRE-EXISTING set {loop, named_recursion, branch, branch_set, branch_get,
 *     jump(loop=true), binding_shadows}. Never constructs a new instruction_kind;
 *     never reads/writes the cpp_* interop family or var_def/var_deref/closure.
 *     => every emitted node already has tested codegen, so codegen/runtime are
 *     unchanged by construction.
 *   - CONSERVATIVE GUARD (spec L3): if a body is not provably loopify-safe, leave it
 *     unchanged. Thus loopify(fn) is observationally equal to fn for all inputs.
 *   - GC DIVIDEND (spec L4): jank's gc_heap + no_refcount_policy means crane's
 *     dangling-shared_ptr hazard (loopify.ml:690-699) cannot occur here; frames are
 *     GC-traced object_refs, so the non-tail case can be MORE complete than crane's.
 */

namespace jank::ir
{
  struct function;

  /* Default-off; gated by util::cli::opts.loopify in processor.cpp. */
  void loopify(function &fn);
}
