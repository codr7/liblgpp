#ifndef LGPP_OPS_BRANCH_GT_HPP
#define LGPP_OPS_BRANCH_GT_HPP

#include <optional>

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"
#include "branch.hpp"

namespace lgpp::ops {
  
  struct BranchGt: Branch {
    BranchGt(lgpp::Label& target, lgpp::Val y, size_t x_offs = 0): Branch(target, y, x_offs) {}

    template <typename T>
    BranchGt(lgpp::Label& target, lgpp::Type<T>& type, T data, size_t x_offs = 0): Branch(target, type, data, x_offs) {}
  };

  template <>
  inline const Op* eval(const Op& op, const BranchGt& imp, lgpp::VM& vm, lgpp::Stack& stack) {
    return (*(stack.end()-imp.x_offs-1) > imp.y) ? &op-op.pc + *imp.target.pc : &op+1;
  }

}

#endif
