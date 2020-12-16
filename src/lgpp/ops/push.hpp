#ifndef LGPP_OPS_PUSH_HPP
#define LGPP_OPS_PUSH_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"
#include "../thread.hpp"

namespace lgpp::ops {
  struct Push {
    template <typename...Args>
    Push(Args&&...args): val(forward<Args>(args)...) {}
      
    Val val;
  };

  template <>
  inline const Op* eval(const Op& op, const Push& imp, Thread& thread) {
    get_stack(thread).push_back(imp.val);
    return &op+1;
  }

}

#endif
