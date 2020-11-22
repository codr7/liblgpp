#include "../label.hpp"
#include "branch_equal.hpp"

namespace lgpp::ops {  
  BranchEqual::BranchEqual(lgpp::Label &target, lgpp::Val x, size_t y_offs): target(target), x(x), y_offs(y_offs) {}

  template <>
  const Op *eval(const Op &op, const BranchEqual &imp, lgpp::Stack &stack) {
    return (imp.x == *(stack.end()-imp.y_offs-1)) ? &op-op.pc+imp.target.pc : &op+1;
  }
}
