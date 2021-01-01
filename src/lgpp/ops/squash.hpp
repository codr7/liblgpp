#ifndef LGPP_OPS_SQUASH_HPP
#define LGPP_OPS_SQUASH_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Squash {};

  template <>
  inline const Op* eval(const Op& op, const Squash& imp, Thread& thread) {
    Stack& in = get_stack(thread), out;
    move(in.begin(), in.end(), back_inserter(out));
    in.clear();
    push(in, thread.vm.Stack, out);
    return &op+1;
  }

}

#endif
