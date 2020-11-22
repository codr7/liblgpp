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
  inline const Op *eval(const Op &op, const Drop &imp, lgpp::VM &vm, lgpp::Stack &stack) {
    auto i = stack.end()-imp.offs-1;
    stack.erase(i, i+imp.len);
    return &op+1;
  }

}

#endif
