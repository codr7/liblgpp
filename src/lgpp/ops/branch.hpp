#ifndef LGPP_OPS_BRANCH_HPP
#define LGPP_OPS_BRANCH_HPP

#include <optional>

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp {
  struct Label;
}

namespace lgpp::ops {
  
  struct Branch {
    Branch(lgpp::Label &target, lgpp::Val x, size_t y_offs = 0): target(target), x(x), y_offs(y_offs) {}

    template <typename T>
    Branch(lgpp::Label &target, lgpp::Type<T> &type, T data, size_t y_offs = 0):
      target(target), x(type, data), y_offs(y_offs) {}
    
    lgpp::Label &target;
    lgpp::Val x;
    size_t y_offs;
  };

}

#endif
