#ifndef LGPP_OPS_TYPE_OF_HPP
#define LGPP_OPS_TYPE_OF_HPP

#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct TypeOf {};

  template <>
  inline const Op* eval(const Op& op, const TypeOf& imp, Thread& thread) {
    auto& s = get_stack(thread);
    push(s, types::Meta, &get_type(pop(s)));
    return &op+1;
  }

}

#endif
