#ifndef LGPP_LABEL_HPP
#define LGPP_LABEL_HPP

#include <cstddef>

namespace lgpp {
  struct Label {
    Label(size_t pc = 0): pc(pc) {}
    size_t pc;
  };
}

#endif
