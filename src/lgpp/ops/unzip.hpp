#ifndef LGPP_OPS_UNZIP_HPP
#define LGPP_OPS_UNZIP_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Unzip {};

  template <>
  inline const Op* eval(const Op& op, const Unzip& imp, Thread& thread) {
    auto& s = get_stack(thread);
    auto p(pop(s).as(thread.vm.Pair));
    push(s, p.first);
    push(s, p.second);
    return &op+1;
  }

}

#endif
