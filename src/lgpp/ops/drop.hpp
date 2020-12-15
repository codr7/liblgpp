#ifndef LGPP_OPS_DROP_HPP
#define LGPP_OPS_DROP_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Drop {
    Drop(size_t offs = 0, size_t len = 1): offs(offs), len(len) {}
    size_t offs, len;
  };

  template <>
  inline const Op* eval(const Op& op, const Drop& imp, Thread& thread) {
    auto& s = get_stack(thread);
    auto i = s.end()-imp.offs-1;
    s.erase(i, i+imp.len);
    return &op+1;
  }

}

#endif
