#ifndef LGPP_OPS_DEC_HPP
#define LGPP_OPS_DEC_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Dec {
    template <typename...Args>
    Dec(Args&&...args): delta(forward<Args>(args)...) {}

    Val delta;
  };

  template <>
  inline const Op* eval(const Op& op, const Dec& imp, lgpp::Thread& thread, lgpp::Stack& stack) {
    push(stack, pop(stack) - imp.delta);
    return &op+1;
  }

}

#endif
