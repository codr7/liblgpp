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
    Branch(lgpp::Label &target, lgpp::Val y, size_t x_offs = 0): target(target), y(y), x_offs(x_offs) {}

    template <typename T>
    Branch(lgpp::Label &target, lgpp::Type<T> &type, T data, size_t x_offs = 0):
      target(target), y(type, data), x_offs(x_offs) {}
    
    lgpp::Label &target;
    lgpp::Val y;
    size_t x_offs;
  };

}

#endif
