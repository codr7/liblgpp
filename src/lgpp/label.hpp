#ifndef LGPP_LABEL_HPP
#define LGPP_LABEL_HPP

#include <cstddef>
#include <optional>

namespace lgpp {
  using namespace std;
  
  struct Label {
    Label(optional<size_t> pc = nullopt): pc(pc) {}
    optional<size_t> pc;
  };
}

#endif
