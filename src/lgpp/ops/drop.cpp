#include "drop.hpp"

namespace lgpp::ops {  
  Drop::Drop(size_t offs, size_t len): offs(offs), len(len) {}

  template <>
  const Op *eval(const Op &op, const Drop &imp, lgpp::Stack &stack) {
    auto i = stack.end()-imp.offs-1;
    stack.erase(i, i+imp.len);
    return &op+1;
  }
}
