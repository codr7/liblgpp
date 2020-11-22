#ifndef LGPP_OPS_BRANCH_EQUAL_HPP
#define LGPP_OPS_BRANCH_EQUAL_HPP

#include <optional>

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp {
  struct Label;
}

namespace lgpp::ops {
  
  struct BranchEqual {
    BranchEqual(lgpp::Label &target, lgpp::Val x, size_t y_offs = 0);

    template <typename T>
    BranchEqual(lgpp::Label &target, lgpp::Type<T> &type, T data, size_t y_offs = 0):
      target(target), x(type, data), y_offs(y_offs) {}
    
    lgpp::Label &target;
    lgpp::Val x;
    size_t y_offs;
  };

  
  template <>
  const Op *eval(const Op &op, const BranchEqual &imp, lgpp::Stack &stack);

}

#endif
