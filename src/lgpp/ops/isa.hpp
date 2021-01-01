#ifndef LGPP_OPS_ISA_HPP
#define LGPP_OPS_ISA_HPP

#include "../val.hpp"
#include "../op.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct Isa {};

  template <>
  inline const Op* eval(const Op& op, const Isa& imp, Thread& thread) {
    auto& s = get_stack(thread);
    auto &parent(*pop(s).as(thread.vm.Meta)), &child(*pop(s).as(thread.vm.Meta));
    auto root = isa(child, parent);
    push(s, thread.vm.Bool, root != nullptr);
    return &op+1;
  }

}

#endif
