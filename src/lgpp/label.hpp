#ifndef LGPP_LABEL_HPP
#define LGPP_LABEL_HPP

#include <cstddef>
#include <optional>
#include <string>

namespace lgpp {
  using namespace std;
  
  struct Label {
    Label(optional<string> name = nullopt, optional<size_t> pc = nullopt): name(name), pc(pc) {}

    optional<string> name;
    optional<size_t> pc;
  };
}

#endif
