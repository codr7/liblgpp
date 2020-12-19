#ifndef LGPP_OPS_LABEL_HPP
#define LGPP_OPS_LABEL_HPP

#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Label {
    Label(PC pc): target(pc) { }
    lgpp::Label target;
  };

  template <>
  inline const Op* eval(const Op& op, const Label& imp, Thread& thread) { return &op+1; }

}

#endif
