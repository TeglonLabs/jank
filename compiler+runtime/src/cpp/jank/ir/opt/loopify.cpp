#include <jank/ir/processor.hpp>
#include <jank/ir/opt/loopify.hpp>

namespace jank::ir
{
  namespace
  {
    /* Does this function recur into itself? `named_recursion` is jank's `recur`
     * target; a plain self `call` is the not-yet-loopified shape crane handles. */
    bool has_self_recursion(function const &fn)
    {
      for(auto const &block : fn.blocks)
      {
        for(auto const &inst : block.instructions)
        {
          if(inst->kind == instruction_kind::named_recursion)
          {
            return true;
          }
        }
      }
      return false;
    }
  }

  /* See loopify.hpp for the full non-cascade contract. This is the conservative
   * skeleton: it DETECTS self-recursion but, until the tail / non-tail rewrites are
   * implemented and differentially tested (loopify-spec.md L5), it DECLINES on every
   * shape and returns identity. Consequences:
   *   - loopify(fn) == fn for all inputs  => zero cascade (the pass is a verified no-op).
   *   - the red test in test/jank/loopify/ fails-until-implemented BY DESIGN (simonw
   *     red/green): enabling --loopify does not yet stop the overflow. */
  void loopify(function &fn)
  {
    if(!has_self_recursion(fn))
    {
      return;
    }

    /* TODO(tail)    classify (spec: crane loopify.ml:806-811). All-tail self-calls
     *   -> rewrite into existing loop + binding_shadows + jump(loop=true). Reuses
     *   jank codegen cpp_processor.cpp:1011 verbatim; ZERO new codegen. */
    /* TODO(nontail) build an explicit frame stack of GC-traced object_refs
     *   (crane _Enter/_Call/_Combine). Safe under gc_heap (spec L4). */
    /* TODO(test)    differential harness f(x) == loopify(f)(x) over a corpus, plus
     *   `first run the tests` baseline against test/bash/clojure-test-suite. */

    /* Conservative: not yet implemented -> identity. */
    return;
  }
}
