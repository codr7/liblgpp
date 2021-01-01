#ifndef LGPP_OPS_RETURN_HPP
#define LGPP_OPS_RETURN_HPP

#include "../op.hpp"
#include "../val.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct Return {};

  template <>
  inline const Op* eval(const Op& op, const Return& imp, Thread& thread) {    
    auto c = pop_call(thread);

    if ((int)c.opts & (int)lgpp::Call::Opts::CORO) {
      auto c = pop_coro(thread);
      if (!c) { throw runtime_error("Missing coro"); }
      assert(!c->done);
      c->pc = op.pc;
      c->done = true;
      push(get_stack(thread), thread.vm.Coro, *c);
    }

    return &op - op.pc + c.return_pc;
  }

}

#endif
