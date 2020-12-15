#ifndef LGPP_OPS_CP_HPP
#define LGPP_OPS_CP_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Cp {
    Cp(size_t offs = 0, size_t len = 1): offs(offs), len(len) {}
    size_t offs, len;
  };

  template <>
  inline const Op* eval(const Op& op, const Cp& imp, Thread& thread) {
    auto& s = get_stack(thread);
    auto i = s.end() - imp.offs - 1;
    copy(i, i + imp.len, back_inserter(s)); 
    return &op+1;
  }

}

#endif
