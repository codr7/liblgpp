#ifndef LGPP_OPS_BRANCH_LT_HPP
#define LGPP_OPS_BRANCH_LT_HPP

#include <optional>

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"
#include "branch.hpp"

namespace lgpp::ops {
  
  struct BranchLt: Branch {
    BranchLt(lgpp::Label &target, lgpp::Val x, size_t y_offs = 0): Branch(target, x, y_offs) {}

    template <typename T>
    BranchLt(lgpp::Label &target, lgpp::Type<T> &type, T data, size_t y_offs = 0): Branch(target, type, data, y_offs) {}
  };

  template <>
  inline const Op *eval(const Op &op, const BranchLt &imp, lgpp::Stack &stack) {
    return (imp.x < *(stack.end()-imp.y_offs-1)) ? &op-op.pc+imp.target.pc : &op+1;
  }

}

#endif
